/**
 * @file htmlgallery.cpp
 * @brief Implements the HTMLGallery class for generating HTML photo galleries.
 */

#include "htmlgallery.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>

/**
 * @brief Constructs an HTMLGallery object.
 * @param parent The parent QObject.
 */
HTMLGallery::HTMLGallery(QObject *parent) : IOutputFile{parent}
{
}

/**
 * @brief Generates and saves the HTML photo gallery.
 *
 * This method performs the following steps:
 * 1. Creates the output directory if it doesn't exist.
 * 2. Processes and saves each image.
 * 3. Generates the HTML content for the gallery.
 * 4. Writes the final HTML file to the output directory.
 */
void HTMLGallery::save()
{
    HTMLGalleryAttributes *attrib = static_cast<HTMLGalleryAttributes*>(&m_Attrib);
    if(attrib->filePath.isEmpty())
    {
        return;
    }

    QString title = attrib->specificSettings["Title"].toString();
    QString description = attrib->specificSettings["Description"].toString();
    int thumbnailSize = attrib->specificSettings["ThumbnailSize"].toInt();
    int columnsCount = attrib->specificSettings["ColumnsCount"].toInt();
    int imageQuality = attrib->specificSettings["ImageQuality"].toInt();
    bool enableLightbox = attrib->specificSettings["EnableLightbox"].toBool();

    // Create output directory

    QDir dir(attrib->filePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // Create images subdirectory
    QDir imagesDir(dir.filePath("images"));
    if (!imagesDir.exists()) {
        imagesDir.mkpath(".");
    }

    QString htmlContent = generateHtmlHeader(title, description, columnsCount);

    // Process images
    emit progressChanged(m_Images.count(), 0);
    for (int i = 0; i < m_Images.size(); ++i) {

        QString imageName = QString("image_%1.jpg").arg(i + 1, 3, 10, QChar('0'));
        QString thumbnailName = QString("thumb_%1.jpg").arg(i + 1, 3, 10, QChar('0'));

        // Save full-size image
        m_Images[i].save(imagesDir.filePath(imageName), "JPG", imageQuality);

        // Create and save thumbnail with adjusted quality
        QImage thumbnail = m_Images[i].scaled(thumbnailSize, thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int adjustedQuality = adjustQualityForThumbnail(thumbnailSize, imageQuality);
        thumbnail.save(imagesDir.filePath(thumbnailName), "JPG", adjustedQuality);

        // Add image to HTML content
        htmlContent += generateImageHtml(imageName, thumbnailName, i, enableLightbox);
        emit progressChanged(m_Images.count(), i+1);
    }

    htmlContent += generateHtmlFooter(enableLightbox);

    // Write HTML file
    QFile file(dir.filePath("index.html"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << htmlContent;
        file.close();
    }
}

/**
 * @brief Generates the HTML header section.
 * @param title The title of the gallery.
 * @param description A brief description of the gallery.
 * @param columnsCount The number of columns in the gallery grid.
 * @return A QString containing the HTML header content, including CSS styles.
 */
QString HTMLGallery::generateHtmlHeader(const QString &title, const QString &description, int columnsCount)
{
    return QString(R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>%1</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f0f0f0; }
        h1 { color: #333; text-align: center; }
        .gallery { display: grid; grid-template-columns: repeat(%3, 1fr); gap: 10px; }
        .gallery-item { position: relative; overflow: hidden; aspect-ratio: 1; }
        .gallery-item img { width: 100%; height: 100%; object-fit: cover; cursor: pointer; }
        .modal { display: none; position: fixed; z-index: 1; padding-top: 50px; left: 0; top: 0; width: 100%; height: 100%; overflow: auto; background-color: rgba(0,0,0,0.9); }
        .modal-content { margin: auto; display: block; width: 80%; max-width: 700px; }
        .close { position: absolute; top: 15px; right: 35px; color: #f1f1f1; font-size: 40px; font-weight: bold; transition: 0.3s; }
        .close:hover, .close:focus { color: #bbb; text-decoration: none; cursor: pointer; }
    </style>
</head>
<body>
    <h1>%1</h1>
    <p>%2</p>
    <div class="gallery">
)").arg(title, description).arg(columnsCount);
}

/**
 * @brief Generates the HTML for an individual image in the gallery.
 * @param imageName The filename of the full-size image.
 * @param thumbnailName The filename of the thumbnail image.
 * @param index The index of the image in the gallery.
 * @param enableLightbox Whether to enable lightbox functionality.
 * @return A QString containing the HTML for the image.
 */
QString HTMLGallery::generateImageHtml(const QString &imageName, const QString &thumbnailName, int index, bool enableLightbox)
{
    if (enableLightbox) {
        return QString(
            "<div class=\"gallery-item\">"
            "<img src=\"images/%2\" alt=\"Gallery Image %3\" onclick=\"openModal('images/%1')\">"
            "</div>"
        ).arg(imageName, thumbnailName).arg(index + 1);
    } else {
        return QString(
            "<div class=\"gallery-item\">"
            "<a href=\"images/%1\" target=\"_blank\">"
            "<img src=\"images/%2\" alt=\"Gallery Image %3\">"
            "</a>"
            "</div>"
        ).arg(imageName, thumbnailName).arg(index + 1);
    }
}

/**
 * @brief Generates the HTML footer section.
 * @param enableLightbox Whether to include lightbox functionality.
 * @return A QString containing the HTML footer content, including JavaScript for the modal functionality if enabled.
 */
QString HTMLGallery::generateHtmlFooter(bool enableLightbox)
{
    QString footer =
        "</div>";

    if (enableLightbox) {
        footer +=
            "<div id=\"imageModal\" class=\"modal\">"
            "<span class=\"close\" onclick=\"closeModal()\">&times;</span>"
            "<img class=\"modal-content\" id=\"modalImage\">"
            "</div>"
            "<style>"
            ".modal { opacity: 0; transition: opacity 0.3s ease; }"
            ".modal.show { opacity: 1; }"
            ".modal-content { transform: scale(0.8); transition: transform 0.3s ease; }"
            ".modal.show .modal-content { transform: scale(1); }"
            "</style>"
            "<script>"
            "function openModal(src) {"
            "    var modal = document.getElementById(\"imageModal\");"
            "    var modalImg = document.getElementById(\"modalImage\");"
            "    modalImg.src = src;"
            "    modal.style.display = \"block\";"
            "    setTimeout(function() { modal.classList.add(\"show\"); }, 10);"
            "}"
            "function closeModal() {"
            "    var modal = document.getElementById(\"imageModal\");"
            "    modal.classList.remove(\"show\");"
            "    setTimeout(function() { modal.style.display = \"none\"; }, 300);"
            "}"
            "</script>";
    }

    footer +=
        "</body>"
        "</html>";

    return footer;
}

/**
 * @brief Adjusts the quality setting for thumbnails based on their size.
 *
 * This method applies the following rules:
 * - For small thumbnails (<=100px): Caps quality at 70% of base quality or 70, whichever is lower.
 * - For medium thumbnails (101-200px): Caps quality at 80% of base quality or 80, whichever is lower.
 * - For large thumbnails (>200px): Uses the full base quality.
 *
 * @param thumbnailSize The size of the thumbnail in pixels.
 * @param baseQuality The base quality setting for images.
 * @return An adjusted quality value for the thumbnail.
 */
int HTMLGallery::adjustQualityForThumbnail(int thumbnailSize, int baseQuality)
{
    // Define quality thresholds
    const int smallThreshold = 100;  // px
    const int mediumThreshold = 200; // px

    // Adjust quality based on thumbnail size
    if (thumbnailSize <= smallThreshold) {
        return qMin(baseQuality, 70);  // Lower quality for small thumbnails
    } else if (thumbnailSize <= mediumThreshold) {
        return qMin(baseQuality, 80);  // Medium quality for medium thumbnails
    } else {
        return baseQuality;  // Full quality for large thumbnails
    }
}
