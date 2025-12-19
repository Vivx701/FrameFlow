#include "frameflowwriter.h"

FrameFlowWriter::FrameFlowWriter(QObject *parent): QObject{parent}
{

}

/**
 * @brief Create a unique pointer to an output file object based on the specified file type.
 * @param fileType The type of output file to create.
 * @return A unique pointer to the created output file object, or nullptr if the file type is not recognized.
 */
std::unique_ptr<IOutputFile> FrameFlowWriter::createOutputFile(const OutputType &fileType)
{
    switch (fileType)
    {
        case OutputType::SPRITE:
            return std::make_unique<ImageSprite>();
            break;
        case OutputType::GIF:
            return std::make_unique<GifFile>();
            break;
        case OutputType::VIDEO:
            return std::make_unique<VideoFile>();
            break;
        default:
              throw FrameFlowException(ERR_INVALID_FILE_TYPE);
    }
    return nullptr;
}
