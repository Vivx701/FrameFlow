#ifndef GIFFILE_H
#define GIFFILE_H

#include "IOutputFile.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
}
class GifFile : public IOutputFile
{
public:
    explicit GifFile(QObject *parent = nullptr);
    void save() override;
};

#endif // GIFFILE_H
