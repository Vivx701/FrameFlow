#ifndef STRINGS_H
#define STRINGS_H

#include <QObject>
#include <QString>

namespace Strings {

// Button texts
inline const QString NEW_BUTTON_TEXT = QObject::tr("New");
inline const QString BROWSE_BUTTON_TEXT = QObject::tr("Browse");
inline const QString EXPORT_BUTTON_TEXT = QObject::tr("Export");

// Tooltips
inline const QString NEW_BUTTON_TOOLTIP = QObject::tr("Start a new project");
inline const QString BROWSE_BUTTON_TOOLTIP = QObject::tr("Add images to project");
inline const QString EXPORT_BUTTON_TOOLTIP = QObject::tr("Export the images");
inline const QString SETTINGS_BUTTON_TOOLTIP = QObject::tr("Settings");
inline const QString ABOUT_BUTTON_TOOLTIP = QObject::tr("About screen");

// Dialog texts
inline const QString OPEN_IMAGE_DIALOG_TITLE = QObject::tr("Open Image");
inline const QString IMAGE_FILES_FILTER = QObject::tr("Image Files (*.png *.jpg *.bmp)");
inline const QString CONFIRM_NEW_TITLE = QObject::tr("Confirm New");
inline const QString CONFIRM_NEW_MESSAGE = QObject::tr("Are you sure you want to start again ?\nAll changes will be lost.");

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

} // namespace Strings

#endif // STRINGS_H
