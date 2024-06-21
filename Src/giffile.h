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
    void save() override;
    void addToFrameData(QImage &img, u_int8_t *frameData);
};

#endif // GIFFILE_H
