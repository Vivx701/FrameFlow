#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imagemodel.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMap>
#include <QLineEdit>
#include <QSpacerItem>
#include <QMessageBox>
#include <QTemporaryDir>
#include <QTranslator>
#include <iconthememanager.h>

#ifdef UNIT_TESTING
class TestFrameFlow;
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupUI();
    void writeFile(Attributes &attrib, OutputType type);
    void resetAll();
    void clearProperties();
    void applySettings();
    QString currentTheme();
    void setLanguage(const QLocale::Language language);

private slots:
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_deleteButton_clicked();
    void on_moveBackButton_clicked();
    void on_moveFrontButton_clicked();

signals:
    void showErrorMessage(QString title, QString msg, QMessageBox::Icon icon);

private:
    Ui::MainWindow *ui;
    ImageModel m_model;
    QGraphicsScene m_scene;
    QGraphicsPixmapItem m_pixmapItem;
    QTemporaryDir *mp_tempDir;
    IconThemeManager *mp_iconThemeManager;
    QTranslator m_translator;

    //toolbuttons
     QToolButton *newButton;
     QToolButton *fileBrowseButton;
     QToolButton *addFromClipboardButton;
     QToolButton *exportButton;
     QToolButton *settingsButton;
     QToolButton *aboutButton;

    // Button texts
    QString NEW_BUTTON_TEXT = QObject::tr("New");
    QString BROWSE_BUTTON_TEXT = QObject::tr("Browse");
    QString EXPORT_BUTTON_TEXT = QObject::tr("Export");
    QString CBOARD_BUTTON_TEXT = QObject::tr("Clipboard");
    QString SETTINGS_BUTTON_TEXT = QObject::tr("Settings");
    QString ABOUT_BUTTON_TEXT = QObject::tr("About");


    QString NEW_BUTTON_TOOLTIP = QObject::tr("Start a new project");
    QString BROWSE_BUTTON_TOOLTIP = QObject::tr("Add images to project");
    QString CLIPBRD_BUTTON_TOOLTIP = QObject::tr("Add image from clipboard");
    QString EXPORT_BUTTON_TOOLTIP = QObject::tr("Export the images");
    QString SETTINGS_BUTTON_TOOLTIP = QObject::tr("Change settings");
    QString ABOUT_BUTTON_TOOLTIP = QObject::tr("About screen");

    // Dialog texts
    QString OPEN_IMAGE_DIALOG_TITLE = QObject::tr("Open Image");
    QString IMAGE_FILES_FILTER = QObject::tr("Image Files (*.png *.jpg *.bmp)");
    QString CONFIRM_NEW_TITLE = QObject::tr("Confirm New");
    QString CONFIRM_NEW_MESSAGE = QObject::tr("Are you sure you want to start again ?\nAll changes will be lost.");
    QString SAVING_MESSAGE = QObject::tr("Saving");
    QString RESTART_MESSAGE = QObject::tr("Please restart this app to Apply language settings");

    //Message Text
    QString NO_CBOARD_TITLE = QObject::tr("No image");
    QString NO_CBOARD_MSG = QObject::tr("No image found in clipboard");
    QString FAILED_CBOARD_TITLE = QObject::tr("Failed");
    QString FAILED_CBOARD_MSG = QObject::tr("Failed to load the image");

    QString NO_IMAGES_TITLE = QObject::tr("No Images Added");
    QString NO_IMAGES_MSG = QObject::tr("Please add at least one image before proceeding");

    #ifdef UNIT_TESTING
    friend class TestFrameFlow;
    #endif

};
#endif // MAINWINDOW_H
