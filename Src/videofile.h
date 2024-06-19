#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include "IOutputFile.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class VideoFile : public IOutputFile
{
public:
    explicit VideoFile(QObject *parent = nullptr);
    void save() override;
    AVFrame* convertQImageToAVFrame(QImage& image, AVCodecContext* codecContext);



private:

};

#endif // VIDEOFILE_H
