#include "giffile.h"

GifFile::GifFile(QObject *parent)
    : IOutputFile{parent}
{

}



AVCodecContext *GifFile::initializeCodecContext(AVCodec *codec, int fps)
{

    //Detect device hardware to allow for set_opt
    av_get_cpu_flags();

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        qDebug() << "Could not allocate video codec context.";
        return nullptr;
    }

    codecContext->width = m_maxFrameWidth;
    codecContext->height = m_maxFrameHeight;
    codecContext->pix_fmt = AV_PIX_FMT_RGB8;
    codecContext->time_base = (AVRational){1, fps};
    codecContext->gop_size = 1;
    codecContext->bit_rate = 400000;
    codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    codecContext->extradata = (uint8_t *)av_mallocz(AV_INPUT_BUFFER_PADDING_SIZE);
    codecContext->extradata_size = AV_INPUT_BUFFER_PADDING_SIZE;

    if (codecContext->extradata) {
                uint8_t *extra = codecContext->extradata;
                extra[0] = 0x21; extra[1] = 0xFF; extra[2] = 0x0B; extra[3] = 'N'; extra[4] = 'E'; extra[5] = 'T';
                extra[6] = 'S'; extra[7] = 'C'; extra[8] = 'A'; extra[9] = 'P'; extra[10] = 'E'; extra[11] = '2';
                extra[12] = '.'; extra[13] = '0'; extra[14] = 3; extra[15] = 1; extra[16] = 0; extra[17] = 0;

    } else {
        qDebug() << "Failed to allocate extradata.";
        avcodec_free_context(&codecContext);
        return nullptr;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        qDebug() << "Error opening codec.";
        avcodec_free_context(&codecContext);
        return nullptr;
    }

    return codecContext;
}

AVFrame *GifFile::initializeFrameFromImage(const QImage &image)
{
    QImage converted = image.convertToFormat(QImage::Format_RGB888);
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        qDebug() << "Could not allocate frame.";
        return nullptr;
    }
    frame->width = converted.width();
    frame->height = converted.height();
    frame->format = AV_PIX_FMT_RGB24;

    if (av_frame_get_buffer(frame, 0) < 0) {
        qDebug() << "Could not allocate 24bit frame data.";
        av_frame_free(&frame);
        return nullptr;
    }

    memcpy(frame->data[0], converted.bits(), converted.sizeInBytes());

    m_maxFrameHeight = qMax(m_maxFrameHeight, converted.height());
    m_maxFrameWidth = qMax(m_maxFrameWidth, converted.width());

    return frame;
}

bool GifFile::encodeFramesAndWriteToFile(const QList<AVFrame *> &frames, AVCodecContext *codecContext, AVFormatContext *formatContext, AVStream *stream)
{
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;

    pkt.data = (uint8_t *)av_malloc(pkt.size);

    if (pkt.data) {
        pkt.data[0] = 0x21; // Extension Introducer
        pkt.data[1] = 0xF9; // Graphic Control Label
        pkt.data[2] = 0x04; // Block Size (fixed size for GCE block)
        pkt.data[3] = 0x00; // Flags (reserved, disposal method, user input flag, transparency flag)
        pkt.data[4] = (uint8_t)(100 & 0xFF); // Delay time low byte
        pkt.data[5] = (uint8_t)((100 >> 8) & 0xFF); // Delay time high byte
        pkt.data[6] = (uint8_t)(0); // Transparent color index low byte
        pkt.data[7] = 0x00; // Terminator
    }

    for (int i = 0; i < frames.length(); ++i) {
        AVFrame* srcFrame = frames[i];
        AVFrame* frame = av_frame_alloc();

        if (!frame) {
            qDebug()<< "Could not allocate frame.";
            return false;
        }
        frame->width = srcFrame->width;
        frame->height = srcFrame->height;
        frame->format = AV_PIX_FMT_RGB8;
        frame->pts = i;

        if (av_frame_get_buffer(frame, 0) < 0) {
            qDebug() << "Could not allocate frame data.";
            av_frame_free(&frame);
            return false;
        }

        SwsContext* swsCtx = sws_getContext(srcFrame->width, srcFrame->height, (AVPixelFormat)srcFrame->format,
                                            frame->width, frame->height, AV_PIX_FMT_RGB8, SWS_BICUBIC, nullptr, nullptr, nullptr);

        if (!swsCtx) {
            qDebug() << "Could not initialize the conversion context.";
            av_frame_free(&frame);
            return false;
        }

        sws_scale(swsCtx, srcFrame->data, srcFrame->linesize, 0, srcFrame->height, frame->data, frame->linesize);

        if (avcodec_send_frame(codecContext, frame) < 0) {
            qDebug() << "Error sending frame for encoding.";
            av_frame_free(&frame);
            sws_freeContext(swsCtx);
            return false;
        }

        while (avcodec_receive_packet(codecContext, &pkt) == 0) {
            pkt.stream_index = stream->index;
            // Set timing information for the packet (if needed)
            if (av_interleaved_write_frame(formatContext, &pkt) < 0) {
                qDebug() << "Error writing packet.";
                av_packet_unref(&pkt);
                av_frame_free(&frame);
                sws_freeContext(swsCtx);
                return false;
            }
            av_packet_unref(&pkt);
        }

        av_frame_free(&frame);
        sws_freeContext(swsCtx);
    }
    return true;
}

void GifFile::setFrameDelayMetadata(AVFrame *frame, int delay)
{

}

void GifFile::save()
{
    GifAttributes *attrib = static_cast<GifAttributes*>(&m_Attrib);
    if(attrib->filePath.isEmpty())
    {
        return;
    }

    QString creator =  attrib->specificSettings["Creator"].toString();
    QString author  =  attrib->specificSettings["Author"].toString();
    int8_t fps      =   attrib->specificSettings["FPS"].toInt();
    int8_t loops     =   attrib->specificSettings["Loops"].toInt();

    avformat_network_init();

    QList<AVFrame*> frames;
    // Load images and initialize frames
    for (const QImage &image : m_Images) {
        AVFrame* frame = initializeFrameFromImage(image);
        if (!frame) {
            return;
        }
        frames.append(frame);
    }

    AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_GIF);
    if (!codec) {
        qDebug() << "Codec not found.";
        return;
    }

    AVCodecContext* codecContext = initializeCodecContext(codec, fps);
    if (!codecContext) {
        return;
    }

    AVFormatContext* formatContext = nullptr;
    avformat_alloc_output_context2(&formatContext, nullptr, "gif", attrib->filePath.toUtf8().data());
    if (!formatContext) {
        qDebug() << "Error creating output format context.";
        avcodec_free_context(&codecContext);
        return;
    }

    AVStream* stream = avformat_new_stream(formatContext, codec);
    if (!stream) {
        qDebug() << "Failed allocating output stream.";
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }
    stream->time_base = (AVRational){1, fps};

    if (avcodec_parameters_from_context(stream->codecpar, codecContext) < 0) {
        qDebug() << "Failed to copy codec parameters to stream.";
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }

    if (avio_open(&formatContext->pb, attrib->filePath.toUtf8().data(), AVIO_FLAG_WRITE) < 0) {
        qDebug() << "Could not open output file.";
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }

    AVDictionary *opts = nullptr;
    av_dict_set(&opts, "loop", QString::number(loops).toUtf8().data(), 0);

    if (avformat_write_header(formatContext, &opts) < 0) {
        qDebug() << "Error occurred when opening output file.";
        avio_close(formatContext->pb);
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }

    if (!encodeFramesAndWriteToFile(frames, codecContext, formatContext, stream)) {
        qDebug() << "Error during frame encoding.";
    }

    av_write_trailer(formatContext);
    avio_close(formatContext->pb);
    avformat_free_context(formatContext);
    avcodec_free_context(&codecContext);

    for (AVFrame *frame : frames) {
        av_frame_free(&frame);
    }
}

