#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include "IOutputFile.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class VideoFile : public IOutputFile
{

public:
    explicit VideoFile(QObject *parent = nullptr);
    void save() override;
    void encodeFrame(AVFormatContext *formatContext, AVCodecContext *codecContext, AVFrame *frame, AVStream *stream);

};

#endif // VIDEOFILE_H
