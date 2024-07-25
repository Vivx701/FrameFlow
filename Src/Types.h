/**
 * @file types.h
 * @brief Defines common types and attributes for various output formats in the FrameFlow application.
 */

#ifndef TYPES_H
#define TYPES_H

#include <QColor>
#include <QString>
#include <QVariantMap>
#include <QImage>
#include <QPageSize>
#include <QDateTime>
#include <QMarginsF>
#include <QPageLayout>
#include <QPagedPaintDevice>
#include <QImage>
#include <QFileInfo>


/// @brief Name of the software
#define SOFTWARENAME "FrameFlow"
/// @brief Version number of the software
#define VERSION 1.0

/**
 * @struct Attributes
 * @brief Base structure for holding common attributes across different output types.
 */
struct Attributes
{
    QColor background;         ///< Background color for the output
    QString filePath;          ///< File path for the output
    QVariantMap specificSettings;  ///< Map of specific settings for each output type
};

/**
 * @struct PDFAttributes
 * @brief Attributes specific to PDF output.
 */
struct PDFAttributes: public Attributes
{
    /**
     * @brief Default constructor for PDFAttributes.
     * Initializes default values for PDF-specific settings.
     */
    PDFAttributes()
    {
        background = QColor::fromRgb(255, 255, 255);
        specificSettings = {
            {"Size", QPageSize::A4},
            {"Creator", SOFTWARENAME},
            {"Margin",  QVariant::fromValue(QMarginsF(10, 10, 10, 10))},
            {"Orientation", QPageLayout::Portrait},
            {"Version", QVariant::fromValue(QPagedPaintDevice::PdfVersion_1_4)},
            {"Title", ""},
            {"Fill", "Center"}
        };
    }

    /// @brief Copy constructor
    PDFAttributes(const PDFAttributes &copy) = default;
};

/**
 * @struct ImageSpriteAttributes
 * @brief Attributes specific to image sprite generation.
 */
struct ImageSpriteAttributes: public Attributes
{
    /**
     * @brief Default constructor for ImageSpriteAttributes.
     * Initializes default values for image sprite-specific settings.
     */
    ImageSpriteAttributes()
    {
        background = QColor::fromRgb(255, 255, 255);
        specificSettings = {
            {"Creator", SOFTWARENAME},
            {"Format",  "PNG"},
            {"Orientation", Qt::Horizontal},
            {"Author", SOFTWARENAME}
        };
    }

    /// @brief Copy constructor
    ImageSpriteAttributes(const ImageSpriteAttributes &copy) = default;
};

/**
 * @struct GifAttributes
 * @brief Attributes specific to GIF output.
 */
struct GifAttributes: public Attributes
{
    /**
     * @brief Default constructor for GifAttributes.
     * Initializes default values for GIF-specific settings.
     */
    GifAttributes()
    {
        background = QColor::fromRgb(255, 255, 255);
        specificSettings = {
            {"Delay",  100}
        };
    }

    /// @brief Copy constructor
    GifAttributes(const GifAttributes &copy) = default;
};

/**
 * @struct VideoAttributes
 * @brief Attributes specific to video output.
 */
struct VideoAttributes: public Attributes
{
    /**
     * @brief Default constructor for VideoAttributes.
     * Initializes default values for video-specific settings.
     */
    VideoAttributes()
    {
        background = QColor::fromRgb(255, 255, 255);
        specificSettings = {
            {"Creator", SOFTWARENAME},
            {"Author", SOFTWARENAME},
            {"FPS",  30},
            {"FrameDelay",  2000},
            {"Title",  "Slideshow"},
            {"Description",  "Slide show created using "+QString(SOFTWARENAME)},
            {"Copyright",  ""}
        };
    }

    /// @brief Copy constructor
    VideoAttributes(const VideoAttributes &copy) = default;
};


/**
 * @struct HTMLGalleryAttributes
 * @brief Attributes specific to HTML gallery generation.
 */
struct HTMLGalleryAttributes: public Attributes
{
    /**
     * @brief Default constructor for HTMLGalleryAttributes.
     * Initializes default values for HTML gallery-specific settings.
     */
    HTMLGalleryAttributes()
    {
        background = QColor::fromRgb(240, 240, 240);  // Light gray background
        specificSettings = {
            {"Creator", SOFTWARENAME},
            {"Author", SOFTWARENAME},
            {"Title", "Photo Gallery"},
            {"Description", "Photo gallery created using " + QString(SOFTWARENAME)},
            {"ThumbnailSize", 200},  // Default thumbnail size in pixels
            {"ColumnsCount", 4},     // Default number of columns in the gallery grid
            {"ImageQuality", 100},    // JPEG quality for saved images (0-100)
            {"Copyright", ""},
            {"EnableLightbox", true} // Enable lightbox for image viewing
        };
    }

    /// @brief Copy constructor
    HTMLGalleryAttributes(const HTMLGalleryAttributes &copy) = default;
};

/**
 * @typedef ImageList
 * @brief A list of QImage objects used for storing multiple images.
 */
typedef QList<QImage> ImageList;

// Custom roles for the image model
enum ImageRoles {
    ImagePathRole = Qt::UserRole + 1,  // Role for storing the image path
    PreviewRole,                       // Role for storing the image preview
    ResolutionRole,                    // Role for storing the image resolution
    FileNameRole                       // Role for storing the image file name
};

// Struct representing an image item in the model
struct ImageItem {
    QImage preview;     // Image preview
    QSize resolution;   // Image resolution
    QFileInfo info;     // Image file information
};

/**
 * @brief Enum representing the different types of output files.
 */
enum OutputType{PDF, VIDEO, GIF, SPRITE, HTMLGALLERY};

//Settings
const QString LANGUAGE = "lang";
const QString ICONTHEME = "icontheme";
const QString COLORTHEME = "theme";
const QString FONT = "fontfamily";
const QString FONTSIZE = "fontsize";

//Theme
const QMap<QString, QString> ThemeMap = {
                                    {"Default", ":/Theme/Resources/Theme/Default.qss"},
                                    {"Light", ":/Theme/Resources/Theme/Light.qss"},
                                    {"Dark", ":/Theme/Resources/Theme/Dark.qss"}
                               };

#endif // TYPES_H
