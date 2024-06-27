#include "imagecollage.h"

/**
 * @brief Constructor for the ImageCollage class.
 * @param parent The parent object.
 */
ImageCollage::ImageCollage(QObject *parent)
    : IOutputFile{parent}
{
    m_Attrib = ImageCollageAttributes();
}

/**
 * @brief Save the images as an image collage.
 *
 * This function generates an image collage by arranging the images stored in the `m_Images` member variable.
 * The collage layout is determined by the specified settings.
 *
 * The function retrieves the necessary settings from the `ImageCollageAttributes` stored in the `m_Attrib` member variable,
 * such as the file path, background color, creator, format, and author.
 *
 * The resulting collage is saved to the specified file path using the provided format and metadata.
 *
 * The function emits the `progressChanged` signal to indicate the progress of generating the collage.
 */
void ImageCollage::save()
{
    ImageCollageAttributes *attrib = static_cast<ImageCollageAttributes*>(&m_Attrib);
    if (attrib->filePath.isEmpty())
    {
        qDebug() << "File path is empty";
        return;
    }
    QString creator = attrib->specificSettings["Creator"].toString();
    QString format = attrib->specificSettings["Format"].toString();
    QString author = attrib->specificSettings["Author"].toString();
    QSize   size = attrib->specificSettings["Size"].value<QSize>();

    QImage collage = generateCollage(attrib->background, size);

    if (collage.isNull())
    {
        qDebug() << "Collage image is NULL";
        return;
    }

    QImageWriter imageWriter(attrib->filePath);
    imageWriter.setFormat(format.toUtf8());
    imageWriter.setText("Author", author);
    imageWriter.setText("Creator", creator);

    if (imageWriter.write(collage))
    {
        qDebug() << "Collage image writing is successful";
    }
    else
    {
        qDebug() << "Collage image writing failed";
    }
}

/**
 * @brief Generate the image collage.
 * @param bgColor The background color of the collage.
 * @return The resulting collage image.
 *
 * This function generates the image collage by arranging the images in a grid layout.
 * The number of rows and columns in the grid is determined based on the number of images.
 * The images are resized to fit within the grid cells while maintaining their aspect ratios.
 *
 * The function emits the `progressChanged` signal to indicate the progress of generating the collage.
 */
QImage ImageCollage::generateCollage(const QColor &bgColor, const QSize &size)
{
    int numImages = m_Images.count();
    int numRows = 2;
    int numCols = 3;

    int padding = 10;
    int cellWidth = (size.width() - (numCols + 1) * padding) / numCols;
    int cellHeight = (size.height() - (numRows + 1) * padding) / numRows;

    QImage collage(size.width(), size.height(), QImage::Format_ARGB32);
    collage.fill(bgColor);

    QPainter painter(&collage);
    painter.setRenderHint(QPainter::Antialiasing);

    int imageIndex = 0;
    emit progressChanged(numImages, 0);

    int top = padding;
    for (int row = 0; row < numRows; ++row)
    {
        int left = padding;
        for (int col = 0; col < numCols; ++col)
        {
            if (imageIndex >= numImages)
            {
                break;
            }

            QImage image = m_Images.at(imageIndex);
            QRect cellRect(left, top, cellWidth, cellHeight);
            QImage scaledImage = image.scaled(cellRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QPoint position = QPoint(cellRect.left() + (cellRect.width() - scaledImage.width()) / 2,
                                     cellRect.top() + (cellRect.height() - scaledImage.height()) / 2);

            painter.drawImage(position, scaledImage);

            left += cellWidth + padding;
            imageIndex++;
            emit progressChanged(numImages, imageIndex);
        }
        top += cellHeight + padding;
    }

    // Add caption
    QString caption = "love, sarah, connor and buddy nelson ♥ New York and twenty-four";
    QFont font("Arial", 12);
    QFontMetrics fontMetrics(font);
    int captionWidth = fontMetrics.horizontalAdvance(caption);
    int captionHeight = fontMetrics.height();

    QPoint captionPosition((collage.width() - captionWidth) / 2, collage.height() - padding - captionHeight);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(captionPosition, caption);

    return collage;
}
