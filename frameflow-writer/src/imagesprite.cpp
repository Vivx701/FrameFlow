#include "imagesprite.h"

/**
 * @brief Constructor for the ImageSprite class.
 * @param parent The parent object.
 */
ImageSprite::ImageSprite(QObject *parent)
    : IOutputFile{parent}
{
    m_Attrib = ImageSpriteAttributes();
}

/**
 * @brief Save the images as a sprite sheet.
 *
 * This function generates a sprite sheet by stitching together the images stored in the `m_Images` member variable.
 * The sprite sheet can be generated in either horizontal or vertical orientation, depending on the specified settings.
 *
 * The function retrieves the necessary settings from the `ImageSpriteAttributes` stored in the `m_Attrib` member variable,
 * such as the file path, background color, creator, format, and author.
 *
 * The resulting sprite sheet is saved to the specified file path using the provided format and metadata.
 *
 * The function emits the `progressChanged` signal to indicate the progress of generating the sprite sheet.
 */
void ImageSprite::save()
{
    ImageSpriteAttributes *attrib = static_cast<ImageSpriteAttributes*>(&m_Attrib);
    if(attrib->filePath.isEmpty())
    {
        throw FrameFlowException(ERR_EMPTY_OUTPUT_PATH);
        return;
    }
    Qt::Orientations orientation = attrib->specificSettings["Orientation"].value<Qt::Orientations>();
    QString creator =  attrib->specificSettings["Creator"].toString();
    QString format  =  attrib->specificSettings["Format"].toString();
    QString author  =  attrib->specificSettings["Author"].toString();

    emit saveStarted(attrib->filePath);
    QImage finalImage;
    switch (orientation) {
    case Qt::Horizontal:
        finalImage = horizontalStitch(attrib->background);
        break;
    case Qt::Vertical:
        finalImage = verticalStitch(attrib->background);
        break;
    }
    if(finalImage.isNull())
    {
        qDebug()<<" Final image is NULL";
        throw FrameFlowException(ERR_UNKNOWN_SAVE);
        return;
    }
    QImageWriter imageWriter(attrib->filePath);
    emit saveStarted(attrib->filePath);
    imageWriter.setFormat(format.toUtf8());
    imageWriter.setText("Author", author);
    imageWriter.setText("Creator", creator);
    if(imageWriter.write(finalImage)){
        qDebug()<<"Image writing is success";
    }else{
        qDebug()<<"Image writing is failed";
        emit saveFinished(false, attrib->filePath);
        throw FrameFlowException(ERR_FRAME_WRITE_FAILED);
        return;
    }
    emit progressChanged(m_Images.count(), m_Images.count());
    emit saveFinished(true, attrib->filePath);
}

/**
 * @brief Stitch the images horizontally to create a sprite sheet.
 * @param bgColor The background color of the sprite sheet.
 * @return The resulting sprite sheet image.
 *
 * This function stitches the images horizontally to create a sprite sheet.
 * It calculates the dimensions of the sprite sheet based on the dimensions of the individual images.
 * The images are then drawn onto the sprite sheet side by side, with the specified background color.
 *
 * The function emits the `progressChanged` signal to indicate the progress of stitching the images.
 */
QImage ImageSprite::horizontalStitch(QColor bgColor)
{
    int imgwidth = 0;
    int imgheight = 0;

    for(QImage img: m_Images){

        imgheight = img.height() > imgheight ? img.height(): imgheight;
        imgwidth += img.width();
    }

    qDebug()<<imgwidth<< imgheight;
    QImage finalImage(imgwidth, imgheight, QImage::Format_ARGB32);
    finalImage.fill(bgColor);
    int drawingIndex = 0;

    emit progressChanged(m_Images.count(), 0);
    for(int i= 0; i<m_Images.count(); i++){
        QImage img = m_Images.at(i);

        for(int y=0;y<img.height(); y++){

            for(int x=0; x<img.width(); x++){

                QRgb color = img.pixel(x, y);
                finalImage.setPixel(drawingIndex+x, y, color);
            }
        }
        drawingIndex += img.width();
        emit progressChanged(m_Images.count(), i);
    }

    return finalImage;
}

/**
 * @brief Stitch the images vertically to create a sprite sheet.
 * @param bgColor The background color of the sprite sheet.
 * @return The resulting sprite sheet image.
 *
 * This function stitches the images vertically to create a sprite sheet.
 * It calculates the dimensions of the sprite sheet based on the dimensions of the individual images.
 * The images are then drawn onto the sprite sheet one below the other, with the specified background color.
 *
 * The function emits the `progressChanged` signal to indicate the progress of stitching the images.
 */
QImage ImageSprite::verticalStitch(QColor bgColor)
{
    int imgwidth = 0;
    int imgheight = 0;

    for(QImage img: m_Images){

        imgwidth = img.width() > imgwidth ? img.width(): imgwidth;
        imgheight += img.height();
    }

    QImage finalImage(imgwidth, imgheight, QImage::Format_ARGB32);
    finalImage.fill(bgColor);
    int drawingIndex = 0;
    emit progressChanged(m_Images.count(), 0);
    for(int i= 0; i<m_Images.count(); i++){
        QImage img = m_Images.at(i);

        for(int y=0;y<img.height(); y++){

            for(int x=0; x<img.width(); x++){

                QRgb color = img.pixel(x, y);
                finalImage.setPixel(x, drawingIndex+y, color);
            }
        }
        drawingIndex += img.height();
        emit progressChanged(m_Images.count(), i);
    }

    return finalImage;
}
