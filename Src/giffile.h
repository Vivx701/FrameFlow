#ifndef GIFFILE_H
#define GIFFILE_H

#include "IOutputFile.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/intreadwrite.h>
}
class GifFile : public IOutputFile
{
public:
    explicit GifFile(QObject *parent = nullptr);
    bool initialize();
    bool addFrame(const QImage& image);
    bool finalize();
    void setExtradata();
    bool addGraphicsControlExtension(AVPacket *pkt);
    void save() override;

private:
    QString m_filename;
    int m_width;
    int m_height;
    int m_fps;
    int m_frameCount = 0;
    int m_delayMs = 100;  // Default delay between frames in milliseconds
    int m_loopCount = 0;  // Default to loop indefinitely (0 means loop forever)

    AVFormatContext* m_formatContext = nullptr;
    AVStream* m_stream = nullptr;
    AVCodec* m_codec = nullptr;
    AVCodecContext* m_codecContext = nullptr;
    AVPacket* m_packet = nullptr;
};

#endif // GIFFILE_H
