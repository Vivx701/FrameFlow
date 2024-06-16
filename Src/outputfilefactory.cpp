#include "outputfilefactory.h"

OutputFileFactory::OutputFileFactory(QObject *parent)
    : QObject{parent}
{

}

std::unique_ptr<IOutputFile> OutputFileFactory::createOutputFile(const OutputType &fileType)
{
    switch (fileType) {
    case OutputType::PDF:
        return std::make_unique<PdfFile>();
        break;
    case OutputType::SPRITE:
        return std::make_unique<ImageSprite>();
        break;
    case OutputType::GIF:
        break;
    case OutputType::VIDEO:
        break;
    default:
        break;
    }

    return nullptr;
}
