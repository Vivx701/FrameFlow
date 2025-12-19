#include "giffile.h"
#include <QDebug>
#include <gif.h>

/**
 * @brief Constructs a GifFile object.
 * @param parent The parent QObject.
 */
GifFile::GifFile(QObject *parent) : IOutputFile{parent} {}


/**
 * @brief Saves the GIF file.
 *
 * This method handles the entire process of creating a GIF file,
 * including initialization, adding frames, finalization, and setting the loop count.
 */
void GifFile::save()
{
    GifAttributes *attrib = static_cast<GifAttributes*>(&m_Attrib);
    if (attrib->filePath.isEmpty())
    {
        qDebug() << "File path is empty";
        throw FrameFlowException(ERR_EMPTY_OUTPUT_PATH);
    }

    QString filename = attrib->filePath;
    int delayMs = attrib->specificSettings["Delay"].toInt();

    QSize maxImageSize = this->getMaxSize();
    int height = maxImageSize.height();
    int width = maxImageSize.width();

    if (width > 65535 || height > 65535) {
        qDebug() << "Image dimensions too large for GIF format";
        throw FrameFlowException(ERR_INVALID_FRAMES);
    }

    GifWriter writer{};
    if (!GifBegin(&writer, filename.toStdString().c_str(), width, height, delayMs / 10, m_depth, true)) {
        qDebug() << "Failed to initialize GIF writer";
        throw FrameFlowException(ERR_WRITER_INIT_FAILED);
    }
    emit saveStarted(filename);
    emit progressChanged(m_Images.count(), 0);
    int8_t index = 0;
    for (const QImage& img : m_Images) {
        QImage rgbImage = img.convertToFormat(QImage::Format_RGB888).scaled(width, height);
        std::vector<uint8_t> frameData(width * height * 4);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                QRgb pixel = rgbImage.pixel(x, y);
                int index = (y * width + x) * 4;
                frameData[index] = qRed(pixel);
                frameData[index + 1] = qGreen(pixel);
                frameData[index + 2] = qBlue(pixel);
                frameData[index + 3] = 255; // Alpha
            }
        }

        if (!GifWriteFrame(&writer, frameData.data(), width, height, delayMs / 10)) {
            qDebug() << "Failed to write frame";
            GifEnd(&writer);
            throw FrameFlowException(ERR_FRAME_WRITE_FAILED);
        }

        emit progressChanged(m_Images.count(), ++index);
    }

    if (!GifEnd(&writer)) {
        qDebug() << "Failed to finalize GIF";
        emit saveFinished(false, filename);
        throw FrameFlowException(ERR_UNKNOWN_SAVE);
    } else {
        qDebug() << "GIF saved successfully";
        emit saveFinished(true, filename);
    }
}
