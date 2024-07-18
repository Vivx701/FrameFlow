#include "giffile.h"

/**
 * @brief Constructs a GifFile object.
 * @param parent The parent QObject.
 */
GifFile::GifFile(QObject *parent)
    : IOutputFile{parent}
{
}

/**
 * @brief Initializes the GIF encoding context and opens the output file.
 * @return true if initialization is successful, false otherwise.
 */
bool GifFile::initialize()
{
    if (m_width > 65535 || m_height > 65535) {
        qDebug() << "Image dimensions too large for GIF format";
        return false;
    }

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
    m_stream->time_base = m_codecContext->time_base;

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

/**
 * @brief Adds a frame to the GIF.
 * @param image The QImage to be added as a frame.
 * @return true if the frame is successfully added, false otherwise.
 */
bool GifFile::addFrame(const QImage &image)
{
    if (image.width() > m_width || image.height() > m_height) {
        qDebug() << "Image dimensions do not match the initialized dimensions";
        return false;
    }
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
    int alignment = 32;  // Use 32-byte alignment for best performance
    int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, m_width, m_height, alignment);
    uint8_t* buffer = (uint8_t*)av_malloc(bufferSize);
    if (!buffer) {
        qDebug() << "Could not allocate buffer for temporary frame";
        av_frame_free(&frame);
        av_frame_free(&tempFrame);
        return false;
    }

    int result = av_image_fill_arrays(tempFrame->data, tempFrame->linesize, buffer,
                                      AV_PIX_FMT_RGBA, m_width, m_height, alignment);
    if (result < 0) {
        qDebug() << "Could not fill image arrays";
        av_free(buffer);
        av_frame_free(&frame);
        av_frame_free(&tempFrame);
        return false;
    }

    // Copy the image data
    for (int y = 0; y < m_height; y++) {
        memcpy(tempFrame->data[0] + y * tempFrame->linesize[0],
               image.constScanLine(y),
               qMin(m_width * 4, tempFrame->linesize[0]));  // Ensure we don't copy more than the line size
    }

    // Set up swscale context
    SwsContext* swsContext = sws_getContext(
        m_width, m_height, AV_PIX_FMT_RGBA,
        m_width, m_height, AV_PIX_FMT_RGB8,
        SWS_LANCZOS, NULL, NULL, NULL
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

    // Set frame timestamp
    frame->pts = m_frameCount;

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

        // Set packet timestamps
        m_packet->pts = m_packet->dts = av_rescale_q(frame->pts, m_codecContext->time_base, m_stream->time_base);
        m_packet->duration = av_rescale_q(1, m_codecContext->time_base, m_stream->time_base);

        if (!addGraphicsControlExtension(m_packet)) {
            qDebug() << "Failed to add Graphics Control Extension";
            av_frame_free(&frame);
            return false;
        }
        m_packet->stream_index = m_stream->index;
        ret = av_interleaved_write_frame(m_formatContext, m_packet);
        if (ret < 0) {
            qDebug() << "Error writing packet: "<<ret;
            av_frame_free(&frame);
            return false;
        }
        av_packet_unref(m_packet);
    }

    m_frameCount++;
    av_frame_free(&frame);
    return true;
}

/**
 * @brief Finalizes the GIF encoding process.
 * @return true if finalization is successful, false otherwise.
 */
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

/**
 * @brief Sets extra data for the codec context.
 */
void GifFile::setExtradata()
{
    m_codecContext->extradata_size = 19;
    m_codecContext->extradata = (uint8_t*)av_mallocz(m_codecContext->extradata_size + AV_INPUT_BUFFER_PADDING_SIZE);
    uint8_t *extra = m_codecContext->extradata;
    extra[0] = 0x21; extra[1] = 0xFF; extra[2] = 0x0B; extra[3] = 'N'; extra[4] = 'E'; extra[5] = 'T';
    extra[6] = 'S'; extra[7] = 'C'; extra[8] = 'A'; extra[9] = 'P'; extra[10] = 'E'; extra[11] = '2';
    extra[12] = '.'; extra[13] = '0'; extra[14] = 3; extra[15] = 1; extra[16] = 0; extra[17] = 0;
}

/**
 * @brief Adds a Graphics Control Extension to a packet.
 * @param pkt The packet to which the GCE should be added.
 * @return true if the GCE is successfully added, false otherwise.
 */
bool GifFile::addGraphicsControlExtension(AVPacket *pkt)
{
    uint16_t delayCentiseconds = m_delayMs / 10;
    uint8_t delayLowByte = delayCentiseconds & 0xFF;
    uint8_t delayHighByte = (delayCentiseconds >> 8) & 0xFF;

    uint8_t gce[8] = {
        0x21, 0xF9, 0x04, 0x00,
        delayLowByte, delayHighByte,
        0x00, 0x00
    };

    if (av_packet_make_writable(pkt) < 0) {
        qDebug() << "Failed to make packet writable";
        return false;
    }

    int new_size = pkt->size + sizeof(gce);
    uint8_t *new_data = (uint8_t *)av_realloc(pkt->data, new_size);
    if (!new_data) {
        qDebug() << "Failed to reallocate packet data";
        return false;
    }

    // Find the start of the image data (usually starts with 0x2C)
    uint8_t *image_start = new_data;
    while (image_start < new_data + pkt->size - 1 && *image_start != 0x2C) {
        image_start++;
    }

    if (image_start == new_data + pkt->size - 1) {
        qDebug() << "Could not find image descriptor";
        return false;
    }

    // Move existing data to make room for GCE
    memmove(image_start + sizeof(gce), image_start, pkt->size - (image_start - new_data));

    // Insert GCE
    memcpy(image_start, gce, sizeof(gce));

    // Update packet size and data
    pkt->size = new_size;
    pkt->data = new_data;

    return true;
}

/**
 * @brief Saves the GIF file.
 *
 * This method initializes the encoder, adds frames, and finalizes the GIF file.
 */
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
