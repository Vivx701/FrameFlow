#include "outputfilefactory.h"

/**
 * @brief Constructor for the OutputFileFactory class.
 * @param parent The parent object.
 */
OutputFileFactory::OutputFileFactory(QObject *parent)
    : QObject{parent}
{
}

/**
 * @brief Create a unique pointer to an output file object based on the specified file type.
 * @param fileType The type of output file to create.
 * @return A unique pointer to the created output file object, or nullptr if the file type is not recognized.
 */
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
        return std::make_unique<GifFile>();
        break;
    case OutputType::VIDEO:
        return std::make_unique<VideoFile>();
        break;
    case OutputType::HTMLGALLERY:
        return std::make_unique<HTMLGallery>();
        break;
    default:
        break;
    }

    return nullptr;
}
