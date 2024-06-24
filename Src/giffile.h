#ifndef GIFFILE_H
#define GIFFILE_H

#include "IOutputFile.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixdesc.h>
}
class GifFile : public IOutputFile
{
public:
    explicit GifFile(QObject *parent = nullptr);
    void save();

private:
    AVCodecContext *initializeCodecContext(AVCodec *codec, int fps);
    void setExtradata(AVCodecContext *codecContext);
    AVFrame *initializeFrameFromImage(const QImage &image);
    bool encodeFramesAndWriteToFile(const QList<AVFrame *> &frames, AVCodecContext *codecContext, AVFormatContext *formatContext, AVStream *stream, int delay);
    bool addGraphicsControlExtension(AVPacket *pkt, int delay);

    int m_maxFrameWidth = 0;
    int m_maxFrameHeight = 0;
};

#endif // GIFFILE_H
