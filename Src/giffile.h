#ifndef GIFFILE_H
#define GIFFILE_H

#include <IOutputFile.h>
#include <QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

/**
 * @class GifFile
 * @brief A class for creating and saving GIF files using FFmpeg libraries.
 *
 * This class inherits from IOutputFile and provides functionality to create
 * GIF files from a series of images, with control over various GIF-specific parameters.
 */
class GifFile : public IOutputFile
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a GifFile object.
     * @param parent The parent QObject.
     */
    explicit GifFile(QObject *parent = nullptr);

    /**
     * @brief Saves the GIF file.
     *
     * This method initializes the encoder, adds frames, and finalizes the GIF file.
     */
    void save() override;

private:
    /**
     * @brief Initializes the GIF encoding context and opens the output file.
     * @return true if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Adds a frame to the GIF.
     * @param image The QImage to be added as a frame.
     * @return true if the frame is successfully added, false otherwise.
     */
    bool addFrame(const QImage &image);

    /**
     * @brief Finalizes the GIF encoding process.
     * @return true if finalization is successful, false otherwise.
     */
    bool finalize();

    /**
     * @brief Sets extra data for the codec context.
     */
    void setExtradata();

    /**
     * @brief Adds a Graphics Control Extension to a packet.
     * @param pkt The packet to which the GCE should be added.
     * @return true if the GCE is successfully added, false otherwise.
     */
    bool addGraphicsControlExtension(AVPacket *pkt);

    AVFormatContext *m_formatContext = nullptr;
    AVStream *m_stream = nullptr;
    const AVCodec *m_codec = nullptr;
    AVCodecContext *m_codecContext = nullptr;
    AVPacket *m_packet = nullptr;

    QString m_filename;
    int m_width = 0;
    int m_height = 0;
    int m_fps = 0;
    int m_loopCount = 0;
    int m_delayMs = 0;
    int64_t m_frameCount = 0;
};

#endif // GIFFILE_H
