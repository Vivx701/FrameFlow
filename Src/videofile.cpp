#include "videofile.h"


VideoFile::VideoFile(QObject *parent): IOutputFile{parent}
{

}

void VideoFile::save()
{
    GifAttributes *attrib = static_cast<GifAttributes*>(&m_Attrib);
    if (attrib->filePath.isEmpty()) {
        return;
    }

    QString creator = attrib->specificSettings["Creator"].toString();
    QString author = attrib->specificSettings["Author"].toString();
    //int fps = attrib->specificSettings["FPS"].toInt();
    int fps = 30;
    int loops = attrib->specificSettings["Loops"].toInt();
    int delay = 100 / fps;  // Calculate delay from fps


    int width = 1920;
    int height = 1080;
    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVStream *videoStream = nullptr;
    AVCodec *codec = nullptr;
    SwsContext *swsContext = nullptr;

    // Initialize format context for AVI
    avformat_alloc_output_context2(&formatContext, nullptr, "avi",  attrib->filePath.toUtf8().data());
    if (!formatContext) {
        qDebug() << "Could not create output context";
        return;
    }

    // Find encoder (using MPEG4 for AVI)
    codec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
    if (!codec) {
        qDebug() << "Codec not found";
        return;
    }

    // Create video stream
    videoStream = avformat_new_stream(formatContext, codec);
    if (!videoStream) {
        qDebug() << "Could not allocate stream";
        return;
    }

    // Allocate codec context
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        qDebug() << "Could not allocate video codec context";
        return;
    }

    // Set codec parameters
    codecContext->bit_rate = 400000;
    codecContext->width = width;
    codecContext->height = height;
    codecContext->time_base = (AVRational){1, 1000};
    codecContext->framerate = (AVRational){1, 1};
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 1;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (formatContext->oformat->flags & AVFMT_GLOBALHEADER)
        codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // Open codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        qDebug() << "Could not open codec";
        return;
    }

    // Copy codec parameters to stream
    avcodec_parameters_from_context(videoStream->codecpar, codecContext);

    // Open output file
    if (!(formatContext->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&formatContext->pb, attrib->filePath.toUtf8().data(), AVIO_FLAG_WRITE) < 0) {
            qDebug() << "Could not open output file";
            return;
        }
    }

    // Write header
    if (avformat_write_header(formatContext, nullptr) < 0) {
        qDebug() << "Error occurred when opening output file";
        return;
    }

    // Initialize swscale context
    swsContext = sws_getContext(width, height, AV_PIX_FMT_RGBA,
                                width, height, AV_PIX_FMT_YUV420P,
                                SWS_BILINEAR, nullptr, nullptr, nullptr);

    // Allocate frame buffer
    AVFrame *frame = av_frame_alloc();
    frame->format = AV_PIX_FMT_YUV420P;
    frame->width = width;
    frame->height = height;
    av_frame_get_buffer(frame, 0);


    // Encode frames
    for (int i = 0; i < m_Images.size(); ++i) {
        const QImage& img = m_Images[i].convertToFormat(QImage::Format_RGBA8888);

        // Convert RGBA to YUV420P
        const uint8_t *inData[1] = { img.bits() };
        int inLinesize[1] = { 4 * width };
        sws_scale(swsContext, inData, inLinesize, 0, height,
                  frame->data, frame->linesize);

        frame->pts = i * 1000;

        // Encode frame
        encodeFrame(formatContext, codecContext, frame, videoStream);
        frame->pts = i * 1000 + 999;  // Just before the next frame
        frame->pict_type = AV_PICTURE_TYPE_P;  // P-frame for efficiency
        encodeFrame(formatContext, codecContext, frame, videoStream);

    }

    // Flush encoder
    encodeFrame(formatContext, codecContext, nullptr, videoStream);

    // Write trailer
    av_write_trailer(formatContext);

    // Clean up
    avcodec_free_context(&codecContext);
    av_frame_free(&frame);
    sws_freeContext(swsContext);
    avio_closep(&formatContext->pb);
    avformat_free_context(formatContext);

}

void VideoFile::encodeFrame(AVFormatContext *formatContext, AVCodecContext *codecContext, AVFrame *frame, AVStream *stream)
{
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;

    int ret = avcodec_send_frame(codecContext, frame);
    if (ret < 0) {
        qDebug() << "Error sending frame for encoding";
        return;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(codecContext, &pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            qDebug() << "Error during encoding";
            break;
        }

        av_packet_rescale_ts(&pkt, codecContext->time_base, stream->time_base);
        pkt.stream_index = stream->index;

        ret = av_interleaved_write_frame(formatContext, &pkt);
        if (ret < 0) {
            qDebug() << "Error writing packet";
            break;
        }
    }

    av_packet_unref(&pkt);
}

