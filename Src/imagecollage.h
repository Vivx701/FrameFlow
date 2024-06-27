#ifndef IMAGECOLLAGE_H
#define IMAGECOLLAGE_H

#include "IOutputFile.h"
#include <QObject>
#include <QImage>
#include <QImageWriter>
#include <QPainter>
#include <QRandomGenerator>
#include <cmath>



/**
 * @brief Class for generating an image collage from a series of images.
 */
class ImageCollage : public IOutputFile
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the ImageCollage class.
     * @param parent The parent object.
     */
    explicit ImageCollage(QObject *parent = nullptr);

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
    void save() override;

signals:
    /**
     * @brief Signal emitted to indicate the progress of generating the collage.
     * @param totalImages The total number of images in the collage.
     * @param imagesProcessed The number of images processed so far.
     */
    void progressChanged(int totalImages, int imagesProcessed);

private:
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
    QImage generateCollage(const QColor &bgColor, const QSize &size);

};

#endif // IMAGECOLLAGE_H
