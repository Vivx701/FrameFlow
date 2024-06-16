#include "imagesprite.h"

ImageSprite::ImageSprite(QObject *parent)
    : IOutputFile{parent}
{
    m_Attrib = ImageSpriteAttributes();
}

void ImageSprite::save()
{
    ImageSpriteAttributes *attrib = static_cast<ImageSpriteAttributes*>(&m_Attrib);
    if(attrib->filePath.isEmpty())
    {
        return;
    }
    Qt::Orientations orientation = attrib->specificSettings["Orientation"].value<Qt::Orientations>();
    QString creator =  attrib->specificSettings["Creator"].toString();
    QString format  =  attrib->specificSettings["Format"].toString();
    QString author  =  attrib->specificSettings["Author"].toString();

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
        return;
    }
    QImageWriter imageWriter(attrib->filePath);

    imageWriter.setFormat(format.toUtf8());
    imageWriter.setText("Author", author);
    imageWriter.setText("Creator", creator);
    if(imageWriter.write(finalImage)){
        qDebug()<<"Image writing is success";
    }else{
        qDebug()<<"Image writing is failed";
    }
}

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

        for(int i=0;i<img.height(); i++){

            for(int j=0; j<img.width(); j++){

                QRgb color = img.pixel(j, i);
                finalImage.setPixel(drawingIndex+j, i, color);
            }
        }
        drawingIndex += img.width();
        emit progressChanged(m_Images.count(), i+1);
    }

    return finalImage;
}

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

        for(int i=0;i<img.height(); i++){

            for(int j=0; j<img.width(); j++){

                QRgb color = img.pixel(j, i);
                finalImage.setPixel(j, drawingIndex+i, color);
            }
        }
        drawingIndex += img.height();
        emit progressChanged(m_Images.count(), i+1);
    }

    return finalImage;
}
