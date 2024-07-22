/**
 * @file htmlgallery.h
 * @brief Defines the HTMLGallery class for generating HTML photo galleries.
 */

#ifndef HTMLGALLERY_H
#define HTMLGALLERY_H

#include "IOutputFile.h"
#include "Types.h"
#include <QString>

/**
 * @class HTMLGallery
 * @brief A class for generating HTML photo galleries from a series of images.
 *
 * This class inherits from IOutputFile and provides functionality to create
 * an interactive HTML photo gallery with a grid layout and optional lightbox image viewer.
 */
class HTMLGallery : public IOutputFile
{


public:
    /**
     * @brief Constructs an HTMLGallery object.
     * @param parent The parent QObject (default is nullptr).
     */
    explicit HTMLGallery(QObject *parent = nullptr);

    /**
     * @brief Generates and saves the HTML photo gallery.
     *
     * This method processes the images, generates the HTML content,
     * and saves it to the specified output path.
     */
    void save() override;

private:
    /**
     * @brief Generates the HTML header section.
     * @param title The title of the gallery.
     * @param description A brief description of the gallery.
     * @param columnsCount The number of columns in the gallery grid.
     * @return A QString containing the HTML header content.
     */
    QString generateHtmlHeader(const QString &title, const QString &description, int columnsCount);

    /**
     * @brief Generates the HTML for an individual image in the gallery.
     * @param imageName The filename of the full-size image.
     * @param thumbnailName The filename of the thumbnail image.
     * @param index The index of the image in the gallery.
     * @param enableLightbox Whether to enable lightbox functionality.
     * @return A QString containing the HTML for the image.
     */
    QString generateImageHtml(const QString &imageName, const QString &thumbnailName, int index, bool enableLightbox);

    /**
     * @brief Generates the HTML footer section.
     * @param enableLightbox Whether to include lightbox functionality.
     * @return A QString containing the HTML footer content, including scripts.
     */
    QString generateHtmlFooter(bool enableLightbox);

    /**
     * @brief Adjusts the quality setting for thumbnails based on their size.
     * @param thumbnailSize The size of the thumbnail in pixels.
     * @param baseQuality The base quality setting for images.
     * @return An adjusted quality value for the thumbnail.
     */
    int adjustQualityForThumbnail(int thumbnailSize, int baseQuality);
};

#endif // HTMLGALLERY_H
