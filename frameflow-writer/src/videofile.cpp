#include "videofile.h"
#include <QDebug>
#include <QDateTime>

/**
 * @brief Constructs a VideoFile object.
 * @param parent The parent QObject.
 */
VideoFile::VideoFile(QObject *parent): IOutputFile{parent}
{
}

/**
 * @brief Saves the video file by encoding a series of images into an MP4 file.
 *
 * This function performs the following steps:
 * 1. Initializes FFmpeg components (codec, format context, stream, etc.)
 * 2. Configures encoding parameters
 * 3. Iterates through input images, encoding each as a video frame
 * 4. Applies specified delay between frames
 * 5. Finalizes the video file and cleans up resources
 */
bool VideoFile::save()
{
    VideoAttributes *attrib = static_cast<VideoAttributes*>(&m_Attrib);
    if (attrib->filePath.isEmpty()) {
        return false;
    }

    QString creator = attrib->specificSettings["Creator"].toString();
    QString author = attrib->specificSettings["Author"].toString();
    QString title = attrib->specificSettings["Title"].toString();
    QString description = attrib->specificSettings["Description"].toString();
    int fps = attrib->specificSettings["FPS"].toInt();
    int frameDelayMs  = attrib->specificSettings["FrameDelay"].toInt();
    QString copyright =  attrib->specificSettings["Copyright"].toString();


    QSize videoSize = this->getMaxSize();
    videoSize.setWidth(videoSize.width() - (videoSize.width() % 2));
    videoSize.setHeight(videoSize.height() - (videoSize.height() % 2));  // height divisible by 2

    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVStream *videoStream = nullptr;
    const AVCodec *codec = nullptr;
    SwsContext *swsContext = nullptr;

    // Initialize format context for MP4
    avformat_alloc_output_context2(&formatContext, nullptr, "mp4", attrib->filePath.toUtf8().data());
    if (!formatContext) {
        qDebug() << "Could not create output context";
         return false;
    }

    // Find H.264 encoder
    codec = avcodec_find_encoder_by_name("libx264");
    if (!codec) {
        qDebug() << "H.264 codec not found";
        return false;
    }

    // Create video stream
    videoStream = avformat_new_stream(formatContext, nullptr);
    if (!videoStream) {
        qDebug() << "Could not allocate stream";
        return false;
    }

    av_dict_set(&formatContext->metadata, "title", title.toUtf8().data(), 0);
    av_dict_set(&formatContext->metadata, "description", description.toUtf8().data(), 0);
    av_dict_set(&formatContext->metadata, "date", QDateTime::currentDateTime().toString(Qt::ISODate).toUtf8().data(), 0);
    av_dict_set(&formatContext->metadata, "year", QString::number(QDateTime::currentDateTime().date().year()).toUtf8().data(), 0);
    av_dict_set(&formatContext->metadata, "year", QString::number(QDateTime::currentDateTime().date().year()).toUtf8().data(), 0);
    av_dict_set(&formatContext->metadata, "encoder", "FFmpeg", 0);
    av_dict_set(&formatContext->metadata, "encoding_tool", creator.toUtf8().data(), 0);
    av_dict_set(&formatContext->metadata, "major_brand", "isom", 0);
    av_dict_set(&formatContext->metadata, "minor_version", "1", 0);
    av_dict_set(&formatContext->metadata, "compatible_brands", "isom,iso2,avc1,mp41", 0);
    av_dict_set(&formatContext->metadata, "copyright", copyright.toUtf8().data(), 0);


    // Allocate codec context
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        qDebug() << "Could not allocate video codec context";
        return false;
    }

    // Set codec parameters
    int64_t bitrate = 0;
    if (videoSize.width() >= 3840 && videoSize.height() >= 2160) {
        // 4K
        bitrate = 35000000; // 35 Mbps
    } else if (videoSize.width() >= 2560 && videoSize.height() >= 1440) {
        // 1440p
        bitrate = 16000000; // 16 Mbps
    } else if (videoSize.width() >= 1920 && videoSize.height() >= 1080) {
        // 1080p
        bitrate = 8000000;  // 8 Mbps
    } else if (videoSize.width() >= 1280 && videoSize.height() >= 720) {
        // 720p
        bitrate = 5000000;  // 5 Mbps
    } else {
        // SD or lower
        bitrate = 2500000;  // 2.5 Mbps
    }

    codecContext->bit_rate = bitrate;
    codecContext->width = videoSize.width();
    codecContext->height = videoSize.height();
    codecContext->time_base = (AVRational){1, 1000}; // Use milliseconds as time base
    codecContext->framerate = (AVRational){fps, 1};
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 1;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    codecContext->codec_type = AVMEDIA_TYPE_VIDEO;

    // Set H.264 specific options
    av_opt_set(codecContext->priv_data, "preset", "medium", 0);
    av_opt_set(codecContext->priv_data, "tune", "zerolatency", 0);

    if (formatContext->oformat->flags & AVFMT_GLOBALHEADER)
        codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // Open codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        qDebug() << "Could not open codec";
        return false;
    }

    // Copy codec parameters to stream
    avcodec_parameters_from_context(videoStream->codecpar, codecContext);

    // Open output file
    if (!(formatContext->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&formatContext->pb, attrib->filePath.toUtf8().data(), AVIO_FLAG_WRITE) < 0) {
            qDebug() << "Could not open output file";
            return false;
        }
    }

    // Write header
    if (avformat_write_header(formatContext, nullptr) < 0) {
        qDebug() << "Error occurred when opening output file";
         return false;
    }

    swsContext = sws_getContext(videoSize.width(), videoSize.height(), AV_PIX_FMT_RGBA,
                                videoSize.width(), videoSize.height(), AV_PIX_FMT_YUV420P,
                                SWS_BILINEAR, nullptr, nullptr, nullptr);

    // Allocate frame buffer
    AVFrame *frame = av_frame_alloc();
    frame->format = AV_PIX_FMT_YUV420P;
    frame->width = videoSize.width();
    frame->height = videoSize.height();
    av_frame_get_buffer(frame, 0);

    int64_t current_pts = 0;

    // Encode frames

    emit progressChanged(m_Images.count()+1, 0);
    for (int i = 0; i < m_Images.size(); ++i) {
        const QImage& img = m_Images[i].convertToFormat(QImage::Format_RGBA8888).scaled(frame->width, frame->height);;

        // Convert RGBA to YUV420P
        const uint8_t *inData[1] = { img.bits() };
        int inLinesize[1] = { 4 * videoSize.width() };
        sws_scale(swsContext, inData, inLinesize, 0, videoSize.height(),
                  frame->data, frame->linesize);

        // Set PTS for the frame
        frame->pts = current_pts;

        // Encode frame
        encodeFrame(formatContext, codecContext, frame, videoStream);

        // Increment PTS by one frame duration
        current_pts += 1000 / fps;

        // Add delay frames
        int delayFrames = (frameDelayMs * fps) / 1000;
        for (int j = 0; j < delayFrames; ++j) {
            frame->pts = current_pts;
            encodeFrame(formatContext, codecContext, frame, videoStream);
            current_pts += 1000 / fps;
        }
        emit progressChanged(m_Images.count()+1, i+1);
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
    emit progressChanged(m_Images.count()+1, m_Images.count()+1);
    qDebug()<<"video saved";
     return true;
}


/**
 * @brief Encodes a single frame and writes it to the output video file.
 *
 * @param formatContext The AVFormatContext for the output file.
 * @param codecContext The AVCodecContext for the video codec.
 * @param frame The AVFrame containing the image data to encode. If nullptr, flushes the encoder.
 * @param stream The AVStream to which the encoded frame will be written.
 *
 * This function performs the following steps:
 * 1. Sends the frame to the encoder
 * 2. Receives encoded packets from the encoder
 * 3. Rescales the packet timestamps
 * 4. Writes the packet to the output file
 */
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

        // Rescale output packet timestamp values from codec to stream timebase
        av_packet_rescale_ts(&pkt, codecContext->time_base, stream->time_base);
        pkt.stream_index = stream->index;

        // Write the compressed frame to the media file
        ret = av_interleaved_write_frame(formatContext, &pkt);
        if (ret < 0) {
            qDebug() << "Error writing packet";
            break;
        }
    }

    av_packet_unref(&pkt);
}
