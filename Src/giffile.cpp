#include "giffile.h"

GifFile::GifFile(QObject *parent)
    : IOutputFile{parent}
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

    //emit progressChanged(m_Images.count(), 0);
    int maxWidth = 0;
    int maxHeight = 0;
    QList<AVFrame*> frames;
    for(int i= 0; i<m_Images.count(); i++){
        QImage converted = m_Images.at(i).convertToFormat(QImage::Format_RGB888);
        AVFrame* frame = av_frame_alloc();
        frame->width = converted.width();
        frame->height = converted.height();
        frame->format = AV_PIX_FMT_RGB24;

        // Allocate memory for frame data
        if (av_frame_get_buffer(frame, 0) < 0) {
            qDebug() << "Could not allocate 24bit frame data.";
            av_frame_free(&frame);
            break;
        }
        memcpy(frame->data[0], converted.bits(), converted.sizeInBytes());
        //emit progressChanged(m_Images.count(), i+1);
        maxHeight = converted.height() > maxHeight ? converted.height(): maxHeight;
        maxWidth = converted.width() > maxWidth ? converted.width(): maxWidth;
        frames.append(frame);

    }

    avformat_network_init();
    //  Initialize codec context and encode frames
    AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_GIF);
    if (!codec) {
        qDebug() << "Codec not found.";
        return;
    }

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
         qDebug() << "Could not allocate video codec context.";
        return;
    }

    // Set codec parameters
    codecContext->width = maxWidth;
    codecContext->height = maxHeight;
    codecContext->pix_fmt = AV_PIX_FMT_RGB8;
    codecContext->time_base = AVRational{1, fps}; // frames per second

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        qDebug() << "Error opening codec.";
        avcodec_free_context(&codecContext);
        return;
    }

    // Open output file
    AVFormatContext* formatContext = nullptr;
    avformat_alloc_output_context2(&formatContext, nullptr, "gif", attrib->filePath.toUtf8());
    if (!formatContext) {
         qDebug() << "Error creating output format context.";
        avcodec_free_context(&codecContext);
        return;
    }

    // Create a new video stream in the output file container
    AVStream* stream = avformat_new_stream(formatContext, codec);
    if (!stream) {
        qDebug()<< "Failed allocating output stream.";
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }

    // Copy the codec parameters from the codec context to the stream
    if (avcodec_parameters_from_context(stream->codecpar, codecContext) < 0) {
        qDebug() << "Failed to copy codec parameters to stream.";
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }

    // Write GIF header
    if (avio_open(&formatContext->pb, attrib->filePath.toUtf8(), AVIO_FLAG_WRITE) < 0) {
        qDebug() << "Could not open output file.";
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }
    if (avformat_write_header(formatContext, nullptr) < 0) {
        qDebug() << "Error occurred when opening output file.";
        avio_close(formatContext->pb);
        avformat_free_context(formatContext);
        avcodec_free_context(&codecContext);
        return;
    }


    // Encode and write frames
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;

    for (int i = 0; i < frames.length(); ++i)
    {
         AVFrame* srcFrame = frames[i];
         AVFrame* frame = av_frame_alloc();

         frame->width = srcFrame->width;
         frame->height = srcFrame->height;
         frame->format = AV_PIX_FMT_RGB8;
         frame->pts = i;
         // Allocate memory for frame data
         if (av_frame_get_buffer(frame, 0) < 0) {
             qDebug() << "Could not allocate frame data.";
             av_frame_free(&frame);
             break;
         }

         // Set up the conversion context
         SwsContext* swsCtx = sws_getContext(srcFrame->width, srcFrame->height, (AVPixelFormat)srcFrame->format,
                                               frame->width, frame->height, AV_PIX_FMT_RGB8, SWS_BICUBIC, nullptr, nullptr, nullptr);

         if (!swsCtx) {
             qDebug()  << "Could not initialize the conversion context.";
             av_frame_free(&frame);
             return;
         }

         // Perform the conversion
         sws_scale(swsCtx, srcFrame->data, srcFrame->linesize, 0, srcFrame->height, frame->data, frame->linesize);

          // Set the presentation timestamp

         if (avcodec_send_frame(codecContext, frame) < 0) {
             qDebug() << "Error sending frame for encoding.";
             av_frame_free(&frame);
             break;
         }

         while (avcodec_receive_packet(codecContext, &pkt) == 0) {
             pkt.stream_index = stream->index;
             if (av_interleaved_write_frame(formatContext, &pkt) < 0) {
                 qDebug() << "Error writing packet.";
                 av_packet_unref(&pkt);
                 break;
             }
             av_packet_unref(&pkt);
         }

         av_frame_free(&frame);

    }

    // Write GIF trailer
    av_write_trailer(formatContext);
    avio_close(formatContext->pb);
    avformat_free_context(formatContext);

    // Clean up
    avcodec_free_context(&codecContext);
    for(AVFrame *frame: frames)
    {
        av_frame_free(&frame);
    }

}

