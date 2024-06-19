#include "giffile.h"

GifFile::GifFile(QObject *parent)
    : IOutputFile{parent}
{

}

void GifFile::save()
{

    avformat_network_init();

    const int width = 1920;
    const int height = 1080;
    const int numFrames = 3;

    uint8_t* frameData[numFrames];
    for (int i = 0; i < numFrames; ++i) {
        frameData[i] = new uint8_t[width * height]; // RGB8 data
        for (int j = 0; j < width * height; ++j) {
            if(i == 0)
            {
                //frameData[i][j] = rand() % 256; // Random color
                frameData[i][j] = 224;
            }
            else if(i==1)
            {
                frameData[i][j] = 28;
            }
            else
            {
                frameData[i][j] = 3;
            }
        }
    }



    // Step 2: Initialize codec context and encode frames
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
    codecContext->width = width;
    codecContext->height = height;
    codecContext->pix_fmt = AV_PIX_FMT_RGB8;
    codecContext->time_base = AVRational{1, 1}; // 10 frames per second

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        qDebug() << "Error opening codec.";
        avcodec_free_context(&codecContext);
        return;
    }

    // Open output file
    AVFormatContext* formatContext = nullptr;
    avformat_alloc_output_context2(&formatContext, nullptr, "gif", "output.gif");
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
    if (avio_open(&formatContext->pb, "output.gif", AVIO_FLAG_WRITE) < 0) {
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

    for (int i = 0; i < numFrames; ++i) {
        AVFrame* frame = av_frame_alloc();
        frame->width = width;
        frame->height = height;
        frame->format = AV_PIX_FMT_RGB8;

        // Allocate memory for frame data
        if (av_frame_get_buffer(frame, 0) < 0) {
            qDebug() << "Could not allocate frame data.";
            av_frame_free(&frame);
            break;
        }

        // Copy random color data to frame
        memcpy(frame->data[0], frameData[i], width * height);

        frame->pts = i; // Set the presentation timestamp

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
    for (int i = 0; i < numFrames; ++i) {
        delete[] frameData[i];
    }

}

