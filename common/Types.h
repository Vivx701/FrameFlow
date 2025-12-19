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
#include <QLocale>
#include <QFontDatabase>

/// @brief Name of the software
#define SOFTWARENAME "FrameFlow"
#define OWNERNAME "Vivx701"
/// @brief Version number of the software
#define VERSION "v0.1.0-alpha"
#define PROJECTFILEVERSION 1
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

/**
 * @brief Enum representing the different types of output files.
 */
enum OutputType{VIDEO, GIF, SPRITE};

// Struct representing an image item in the model
struct ImageItem {
    QImage preview;     // Image preview
    QSize resolution;   // Image resolution
    QFileInfo info;     // Image file information
};

/**
 * @brief Semantic identifiers for project  keys.
 * */
constexpr const char* KEY_IMAGES     = "IMAGES";
constexpr const char* KEY_INDEX      = "CURRENTINDEX";
constexpr const char* KEY_LASTSAVED  = "LASTSAVED";
constexpr const char* KEY_VERSION    = "VERSION";


/**
 * @brief identifiers for settings  keys.
 * */
constexpr const char* SETTINGS_LANG      = "LANGUAGE";
constexpr const char* SETTINGS_ICON      = "ICONTHEME";
constexpr const char* SETTINGS_STYLE     = "STYLETHEME";

#endif // TYPES_H
