#include "giffile.h"
#include <QDebug>
#include <gif.h>

GifFile::GifFile(QObject *parent) : IOutputFile{parent} {}

void GifFile::save()
{
    GifAttributes *attrib = static_cast<GifAttributes*>(&m_Attrib);
    if (attrib->filePath.isEmpty()) {
        qDebug() << "File path is empty";
        return;
    }

    QString filename = attrib->filePath;
    int loopCount = attrib->specificSettings["Loops"].toInt();
    int delayMs = attrib->specificSettings["Delay"].toInt();

    QSize maxImageSize = this->getMaxSize();
    int height = maxImageSize.height();
    int width = maxImageSize.width();

    if (width > 65535 || height > 65535) {
        qDebug() << "Image dimensions too large for GIF format";
        return;
    }

    GifWriter writer{};
    if (!GifBegin(&writer, filename.toStdString().c_str(), width, height, delayMs / 10, 8, true)) {
        qDebug() << "Failed to initialize GIF writer";
        return;
    }

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
            return;
        }
    }

    if (!GifEnd(&writer)) {
        qDebug() << "Failed to finalize GIF";
    } else {
        qDebug() << "GIF saved successfully";
    }
}
