#ifndef STRINGS_H
#define STRINGS_H
#include <QObject>


#define SOFTWARE_TITLE  QObject::tr("FrameFlow")

// ---------------- Dialog texts ----------------

#define OPEN_IMAGE_DIALOG_TITLE \
QObject::tr("Open Image")

#define IMAGE_FILES_FILTER \
    QObject::tr("Image Files (*.png *.jpg *.bmp)")

#define PROJECT_FILES_FILTER \
    QObject::tr("PROJECT (*.ffp)")

#define CONFIRM_NEW_TITLE \
    QObject::tr("Confirm New")

#define CONFIRM_NEW_MESSAGE \
    QObject::tr("Are you sure you want to start again? All changes will be lost.")

#define SAVING_MESSAGE \
    QObject::tr("Saving")

#define RESTART_MESSAGE \
    QObject::tr("Please restart this app to apply language settings")

// ---------------- Clipboard messages ----------------

#define NO_CBOARD_TITLE \
    QObject::tr("No Image")

#define NO_CBOARD_MSG \
    QObject::tr("No image found in clipboard")

#define FAILED_CBOARD_TITLE \
    QObject::tr("Failed")

#define FAILED_CBOARD_MSG \
    QObject::tr("Failed to load the image")

// ---------------- Image / export messages ----------------

#define NO_IMAGES_TITLE \
    QObject::tr("No Images Added")

#define NO_IMAGES_MSG \
    QObject::tr("Please add at least one image before proceeding")

#define NO_IMAGES_SELECTED \
    QObject::tr("No Image selected")

#define EXPORT_FAILED \
    QObject::tr("Failed to export")

#define EXPORT_SUCCESS \
    QObject::tr("Export success")

#define EXPORT_COMPLETED \
    QObject::tr("Export completed")
#define EXPORT_STARTED \
    QObject::tr("Writing output file")
#define ERR_FILE_SAVE_FAILED \
    QObject::tr("File save failed.")

// ---------------- Project / save errors ----------------

#define SAVE_FAILED_TITLE \
    QObject::tr("Save Failed")

#define SAVE_OPEN_FAILED_MSG \
    QObject::tr("Unable to save the project file.The file could not be opened for writing.")

#define SAVE_WRITE_FAILED_MSG \
    QObject::tr("Failed to save the project file.Not all data could be written to disk.")

#define SAVE_VERIFY_FAILED_MSG \
    QObject::tr("Failed to save the project file.The written file could not be verified.")

#define SAVE_SUCCESS_MSG \
    QObject::tr("Project file saved successfully.")

#define SAVE_SUCCESS_TITLE  QObject::tr("Save Successful")

// ---------------- Project / loading ----------------

#define LOADING_MESSAGE \
QObject::tr("Loading")

#define LOAD_PROJECT_TITLE \
    QObject::tr("Load Project")

#define LOAD_FAILED_TITLE \
    QObject::tr("Load Failed")

#define LOAD_OPEN_FAILED_MSG \
    QObject::tr("Unable to load the project file.The file could not be opened.")

#define LOAD_PARSE_FAILED_MSG \
    QObject::tr("Failed to load the project file.The file format is invalid or corrupted.")

#define LOAD_VERSION_UNSUPPORTED_MSG \
    QObject::tr("Failed to load the project file.This project version is not supported.")

#define LOAD_IMAGES_MISSING_MSG \
    QObject::tr("The project was loaded, but some images could not be found. Missing images will be skipped.")


#define LOAD_SUCCESS_MSG \
    QObject::tr("Project file loaded successfully.")

/* ---------- Theme Messages ---------- */

#define THEME_APPLY_SUCCESS_TITLE \
QObject::tr("Theme Applied")

#define THEME_APPLY_SUCCESS_MSG \
    QObject::tr("The selected theme was applied successfully.")

#define THEME_APPLY_FAILED_TITLE \
    QObject::tr("Theme Apply Failed")

#define THEME_APPLY_FAILED_MSG \
    QObject::tr("Failed to apply the selected theme.")

//--------Export Dialogs-----------------------
#define VIDEO_FPS_INVALID_MSG \
    QObject::tr("FPS must be greater than zero.")

#define VIDEO_FRAME_DELAY_INVALID_MSG \
    QObject::tr("Frame delay must be greater than zero.")

#define VIDEO_ATTRIBUTES_INVALID_TITLE \
    QObject::tr("Invalid Video Attributes")

#define VIDEO_EXPORT_PATH_MISSING_MSG \
    QObject::tr("Please select an export file location.")

#define VIDEO_EXPORT_PATH_INVALID_MSG \
    QObject::tr("The selected export file path is invalid or not writable.")

#define VIDEO_ATTRIBUTES_INVALID_TITLE \
    QObject::tr("Invalid Video Attributes")

#define VIDEO_EXPORT_DIALOG_TITLE \
QObject::tr("Export Video")

#define VIDEO_EXPORT_FILE_FILTER \
    QObject::tr("Video Files (*.mp4)")


#define GIF_DELAY_INVALID_MSG \
QObject::tr("GIF frame delay must be greater than zero.")

#define GIF_EXPORT_PATH_MISSING_MSG \
    QObject::tr("Please select an export GIF file location.")

#define GIF_EXPORT_PATH_INVALID_MSG \
    QObject::tr("The selected export GIF file path is invalid or not writable.")

#define GIF_ATTRIBUTES_INVALID_TITLE \
    QObject::tr("Invalid GIF Attributes")

#define GIF_EXPORT_DIALOG_TITLE \
QObject::tr("Export GIF")

#define GIF_EXPORT_FILE_FILTER \
    QObject::tr("GIF Files (*.gif)")

//Sprite images

#define SPRITE_EXPORT_DIALOG_TITLE \
QObject::tr("Export Image Sprite")

#define SPRITE_EXPORT_FILTER_PNG \
    QObject::tr("PNG Image (*.png)")

#define SPRITE_EXPORT_FILTER_JPG \
    QObject::tr("JPEG Image (*.jpg)")

#define SPRITE_EXPORT_FILTER_BMP \
    QObject::tr("Bitmap Image (*.bmp)")


#define SPRITE_ATTRIBUTES_INVALID_TITLE \
QObject::tr("Invalid Image Sprite Attributes")

#define SPRITE_EXPORT_PATH_MISSING_MSG \
    QObject::tr("Please select an export image sprite file location.")

#define SPRITE_EXPORT_PATH_INVALID_MSG \
    QObject::tr("The selected export file path is invalid or not writable.")

#define EXPORT_CANCEL_MSG \
    QObject::tr("Please wait while the operation is being cancelled.")

#endif // STRINGS_H
