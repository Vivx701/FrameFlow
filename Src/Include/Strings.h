#ifndef STRINGS_H
#define STRINGS_H

#include <QObject>
#include <QString>

namespace Strings {

// Button texts
inline const QString NEW_BUTTON_TEXT = QObject::tr("New");
inline const QString BROWSE_BUTTON_TEXT = QObject::tr("Browse");
inline const QString EXPORT_BUTTON_TEXT = QObject::tr("Export");
inline const QString CBOARD_BUTTON_TEXT = QObject::tr("Clipboard");
inline const QString SETTINGS_BUTTON_TEXT = QObject::tr("Settings");
inline const QString ABOUT_BUTTON_TEXT = QObject::tr("About");

// Tooltips
inline const QString NEW_BUTTON_TOOLTIP = QObject::tr("Start a new project");
inline const QString BROWSE_BUTTON_TOOLTIP = QObject::tr("Add images to project");
inline const QString CLIPBRD_BUTTON_TOOLTIP = QObject::tr("Add image from clipboard");
inline const QString EXPORT_BUTTON_TOOLTIP = QObject::tr("Export the images");
inline const QString SETTINGS_BUTTON_TOOLTIP = QObject::tr("Change settings");
inline const QString ABOUT_BUTTON_TOOLTIP = QObject::tr("About screen");

// Dialog texts
inline const QString OPEN_IMAGE_DIALOG_TITLE = QObject::tr("Open Image");
inline const QString IMAGE_FILES_FILTER = QObject::tr("Image Files (*.png *.jpg *.bmp)");
inline const QString CONFIRM_NEW_TITLE = QObject::tr("Confirm New");
inline const QString CONFIRM_NEW_MESSAGE = QObject::tr("Are you sure you want to start again ?\nAll changes will be lost.");
inline const QString SAVING_MESSAGE = QObject::tr("Saving");

// Property names
inline const QString FILENAME_PROPERTY = QObject::tr("Filename");
inline const QString DIR_PROPERTY = QObject::tr("Dir");
inline const QString BYTES_PROPERTY = QObject::tr("Bytes");
inline const QString MODIFIED_PROPERTY = QObject::tr("Modified");
inline const QString WIDTH_PROPERTY = QObject::tr("Width");
inline const QString HEIGHT_PROPERTY = QObject::tr("Height");
inline const QString DEPTH_PROPERTY = QObject::tr("Depth");
inline const QString FORMAT_PROPERTY = QObject::tr("Format");
inline const QString COLOR_COUNT_PROPERTY = QObject::tr("Color Count");
inline const QString IS_NULL_PROPERTY = QObject::tr("Is Null");
inline const QString BYTES_PER_LINE_PROPERTY = QObject::tr("Bytes Per Line");
inline const QString DEVICE_PIXEL_RATIO_PROPERTY = QObject::tr("Device Pixel Ratio");
inline const QString DOTS_PER_METER_X_PROPERTY = QObject::tr("DotsPerMeterX");
inline const QString DOTS_PER_METER_Y_PROPERTY = QObject::tr("DotsPerMeterY");
inline const QString OFFSET_PROPERTY = QObject::tr("Offset");
inline const QString IS_GRAYSCALE_PROPERTY = QObject::tr("Is Grayscale");
inline const QString HAS_ALPHA_CHANNEL_PROPERTY = QObject::tr("Has Alpha Channel");
inline const QString BIT_PLANE_COUNT_PROPERTY = QObject::tr("Bit Plane Count");
inline const QString INDEX_PROPERTY = QObject::tr("Index");

// Unit strings
inline const QString PIXELS_UNIT = QObject::tr("px");
inline const QString BITS_UNIT = QObject::tr("bits");

// Yes/No strings
inline const QString YES = QObject::tr("Yes");
inline const QString NO = QObject::tr("No");


//Message Text
inline const QString NO_CBOARD_TITLE = QObject::tr("No image");
inline const QString NO_CBOARD_MSG = QObject::tr("No image found in clipboard");

inline const QString FAILED_CBOARD_TITLE = QObject::tr("Failed");
inline const QString FAILED_CBOARD_MSG = QObject::tr("Failed to load the image");

inline const QString NO_IMAGES_TITLE = QObject::tr("No Images Added");
inline const QString NO_IMAGES_MSG = QObject::tr("Please add at least one image before proceeding");

// About Dialog
inline const QString ABOUT_DIALOG_TITLE = QObject::tr("About FrameFlow");
inline const QString APP_NAME = QObject::tr("FrameFlow");
inline const QString APP_VERSION = QObject::tr("Version 1.0");
inline const QString APP_DESCRIPTION = QObject::tr("FrameFlow is a versatile application designed to transform series of images into various multimedia formats. With FrameFlow, you can easily create videos, PDFs, sprite images, GIFs, and HTML presentations from your image sequences. Whether you're a designer, animator, or content creator, FrameFlow streamlines your workflow for efficient and creative visual storytelling.");
inline const QString APP_COPYRIGHT = QObject::tr("Copyright © 2024 vivx_developer (https://github.com/Vivx701/FrameFlow)");
inline const QString FFMPEG_REFERENCE = QObject::tr("This application uses FFmpeg for video processing.");
inline const QString GIFH_REFERENCE = QObject::tr("GIF encoding powered by gif-h");
inline const QString GIFH_LINK = QObject::tr("https://github.com/charlietangora/gif-h");
inline const QString QT_VERSION_BUTTON_TEXT = QObject::tr("Show Qt Version");
inline const QString CLOSE_BUTTON_TEXT = QObject::tr("Close");
inline const QString QT_VERSION_DIALOG_TITLE = QObject::tr("Qt Version");
inline const QString QT_VERSION_MESSAGE = QObject::tr("This application is using Qt version %1");

} // namespace Strings

#endif // STRINGS_H
