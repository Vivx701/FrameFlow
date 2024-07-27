#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include "IOutputFile.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

/**
 * @brief The VideoFile class handles the creation of video files from a series of images.
 *
 * This class inherits from IOutputFile and uses FFmpeg libraries to encode
 * images into a video file format (MP4 with H.264 encoding).
 */
class VideoFile : public IOutputFile
{
public:
    /**
     * @brief Constructs a VideoFile object.
     * @param parent The parent QObject (optional).
     */
    explicit VideoFile(QObject *parent = nullptr);

    /**
     * @brief Saves the video file by encoding a series of images into an MP4 file.
     *
     * This function overrides the save() method from IOutputFile.
     */
    void save() override;


private:
    /**
     * @brief Encodes a single frame and writes it to the output video file.
     *
     * @param formatContext The AVFormatContext for the output file.
     * @param codecContext The AVCodecContext for the video codec.
     * @param frame The AVFrame containing the image data to encode. If nullptr, flushes the encoder.
     * @param stream The AVStream to which the encoded frame will be written.
     */
    void encodeFrame(AVFormatContext *formatContext, AVCodecContext *codecContext, AVFrame *frame, AVStream *stream);
};

#endif // VIDEOFILE_H
