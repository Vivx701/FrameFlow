#include "videofile.h"

VideoFile::VideoFile(QObject *parent)
    : IOutputFile{parent}
{

}

void VideoFile::save()
{


}

AVFrame *VideoFile::convertQImageToAVFrame(QImage &image, AVCodecContext *codecContext)
{

}
