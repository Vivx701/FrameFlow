#include "giffile.h"

GifFile::GifFile(QObject *parent)
    : IOutputFile{parent}
{

}

bool GifFile::initialize()
{
    avformat_alloc_output_context2(&m_formatContext, NULL, NULL, m_filename.toStdString().c_str());
    if (!m_formatContext) {
        qDebug() << "Could not create output context";
        return false;
    }

    m_stream = avformat_new_stream(m_formatContext, NULL);
    if (!m_stream) {
        qDebug() << "Failed allocating output stream";
        return false;
    }

    m_codec = avcodec_find_encoder(AV_CODEC_ID_GIF);
    if (!m_codec) {
        qDebug() << "Codec not found";
        return false;
    }

    m_codecContext = avcodec_alloc_context3(m_codec);
    if (!m_codecContext) {
        qDebug() << "Could not allocate video codec context";
        return false;
    }

    m_packet = av_packet_alloc();
    if (!m_packet) {
        qDebug() << "Could not allocate packet";
        return false;
    }

    m_codecContext->bit_rate = 1200000;
    m_codecContext->width = m_width;
    m_codecContext->height = m_height;
    m_codecContext->time_base = (AVRational){1, m_fps};
    m_codecContext->pix_fmt = AV_PIX_FMT_RGB8;

    // Set GIF-specific options
    av_opt_set(m_codecContext->priv_data, "loop", std::to_string(m_loopCount).c_str(), 0);
    av_opt_set(m_codecContext->priv_data, "delay", std::to_string(m_delayMs).c_str(), 0);

    if (avcodec_open2(m_codecContext, m_codec, NULL) < 0) {
        qDebug() << "Could not open codec";
        return false;
    }

    avcodec_parameters_from_context(m_stream->codecpar, m_codecContext);

    if (avio_open(&m_formatContext->pb, m_filename.toStdString().c_str(), AVIO_FLAG_WRITE) < 0) {
        qDebug() << "Could not open output file";
        return false;
    }

    if (avformat_write_header(m_formatContext, NULL) < 0) {
        qDebug() << "Error occurred when opening output file";
        return false;
    }

    return true;
}

bool GifFile::addFrame(const QImage &image)
{
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        qDebug() << "Could not allocate video frame";
        return false;
    }

    frame->format = AV_PIX_FMT_RGB8;
    frame->width  = m_width;
    frame->height = m_height;

    if (av_frame_get_buffer(frame, 32) < 0) {
        qDebug() << "Could not allocate the video frame data";
        av_frame_free(&frame);
        return false;
    }

    // Create temporary frame for input image
    AVFrame* tempFrame = av_frame_alloc();
    if (!tempFrame) {
        qDebug() << "Could not allocate temporary video frame";
        av_frame_free(&frame);
        return false;
    }

    // Prepare temporary frame from QImage
    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_width, m_height, 1);
    uint8_t* buffer = (uint8_t*)av_malloc(bufferSize);
    av_image_fill_arrays(tempFrame->data, tempFrame->linesize, buffer,
                         AV_PIX_FMT_RGBA, m_width, m_height, 1);

    // Copy QImage data to temporary frame
    for (int y = 0; y < m_height; y++) {
        memcpy(tempFrame->data[0] + y * tempFrame->linesize[0],
               image.constBits() + y * image.bytesPerLine(),
               m_width * 4);
    }

    // Set up swscale context
    SwsContext* swsContext = sws_getContext(
        m_width, m_height, AV_PIX_FMT_RGBA,
        m_width, m_height, AV_PIX_FMT_RGB8,
        SWS_BILINEAR, NULL, NULL, NULL
    );

    if (!swsContext) {
        qDebug() << "Could not initialize the conversion context";
        av_frame_free(&frame);
        av_frame_free(&tempFrame);
        av_free(buffer);
        return false;
    }

    // Perform the color conversion
    sws_scale(swsContext, tempFrame->data, tempFrame->linesize,
              0, m_height, frame->data, frame->linesize);

    // Clean up
    sws_freeContext(swsContext);
    av_frame_free(&tempFrame);
    av_free(buffer);

    frame->pts = m_frameCount++;



//    // Set frame-specific delay if provided
//    if (m_delayMs > 0) {
//        char delay_str[32];
//        snprintf(delay_str, sizeof(delay_str), "%d", m_delayMs);
//        av_dict_set(&frame->metadata, "delay", delay_str, 0);
//    }

    int ret = avcodec_send_frame(m_codecContext, frame);
    if (ret < 0) {
        qDebug() << "Error sending a frame for encoding";
        av_frame_free(&frame);
        return false;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(m_codecContext, m_packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            qDebug() << "Error during encoding";
            av_frame_free(&frame);
            return false;
        }

        av_packet_rescale_ts(m_packet, m_codecContext->time_base, m_stream->time_base);
        m_packet->stream_index = m_stream->index;
        ret = av_interleaved_write_frame(m_formatContext, m_packet);
        av_packet_unref(m_packet);
    }

    av_frame_free(&frame);
    return true;

}

bool GifFile::finalize()
{
    // Flush the encoder
    avcodec_send_frame(m_codecContext, NULL);
    while (true) {
        int ret = avcodec_receive_packet(m_codecContext, m_packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            qDebug() << "Error during flushing";
            return false;
        }

        av_packet_rescale_ts(m_packet, m_codecContext->time_base, m_stream->time_base);
        m_packet->stream_index = m_stream->index;

        ret = av_interleaved_write_frame(m_formatContext, m_packet);
        av_packet_unref(m_packet);
    }

    av_write_trailer(m_formatContext);
    avcodec_free_context(&m_codecContext);
    av_packet_free(&m_packet);
    avio_closep(&m_formatContext->pb);
    avformat_free_context(m_formatContext);

    return true;
}

void GifFile::setExtradata()
{
    m_codecContext->extradata_size = 19;
    m_codecContext->extradata = (uint8_t*)av_mallocz(m_codecContext->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
    uint8_t *extra = m_codecContext->extradata;
    extra[0] = 0x21; extra[1] = 0xFF; extra[2] = 0x0B; extra[3] = 'N'; extra[4] = 'E'; extra[5] = 'T';
    extra[6] = 'S'; extra[7] = 'C'; extra[8] = 'A'; extra[9] = 'P'; extra[10] = 'E'; extra[11] = '2';
    extra[12] = '.'; extra[13] = '0'; extra[14] = 3; extra[15] = 1; extra[16] = 0; extra[17] = 0;
}

bool GifFile::addGraphicsControlExtension(AVPacket *pkt)
{
    // Ensure delay is within uint8_t range
    uint8_t delayLowByte = m_delayMs & 0xFF;       // Extract low byte
    uint8_t delayHighByte = (m_delayMs >> 8) & 0xFF; // Extract high byte

    // GCE block structure
    uint8_t gce[8] = {
        0x21,   // Extension Introducer
        0xF9,   // Graphic Control Label
        0x04,   // Block Size
        0x00,   // Flags: No transparency, no user input, disposal method 0
        delayLowByte,     // Delay time low byte
        delayHighByte,    // Delay time high byte
        0x00,   // Transparent color index
        0x00    // Block Terminator
    };

    // Reallocate packet data with additional space for GCE
    uint8_t *new_data = (uint8_t *)av_realloc(pkt->data, pkt->size + sizeof(gce));
    if (!new_data) {
        qDebug() << "Failed to reallocate packet data.";
        return false;
    }

    pkt->data = new_data;

    // Move existing packet data to make space for GCE
    memmove(pkt->data + sizeof(gce), pkt->data, pkt->size);
    // Copy GCE to the beginning of the packet data
    memcpy(pkt->data, gce, sizeof(gce));

    // Update packet size to include GCE
    pkt->size += sizeof(gce);

    // Verification: Check if the first bytes match the GCE
    if (memcmp(pkt->data, gce, sizeof(gce)) != 0) {
        qDebug() << "GCE verification failed.";
        return false;
    }

    return true;
}


void GifFile::save()
{
    GifAttributes *attrib = static_cast<GifAttributes*>(&m_Attrib);
    if (attrib->filePath.isEmpty()) {
        return;
    }
    m_filename = attrib->filePath;
    QString creator = attrib->specificSettings["Creator"].toString();
    QString author = attrib->specificSettings["Author"].toString();
    m_fps = attrib->specificSettings["FPS"].toInt();
    m_loopCount = attrib->specificSettings["Loops"].toInt();
    m_delayMs = attrib->specificSettings["Delay"].toInt();

    qDebug()<<m_loopCount<<":"<<m_delayMs;
    QSize maxImageSize = this->getMaxSize();
    m_height = maxImageSize.height();
    m_width = maxImageSize.width();

    if(!initialize())
    {
        qDebug()<< "Codec initialize failed";
        return;
    }

    // Encode frames
    for (int i = 0; i < m_Images.size(); ++i) {
        const QImage& img = m_Images[i].convertToFormat(QImage::Format_RGBA8888);
        if (!addFrame(img)) {
            qDebug() << "Failed to add frame";
            return;
        }
    }

    // Finalize the GIF
    if (!finalize()) {
        qDebug() << "Failed to finalize encoder";
    }

}
