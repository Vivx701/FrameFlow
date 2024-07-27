#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QFont>
#include <QTimer>
#include <FrameFlowSplashScreen.h>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("QT_IMAGEIO_MAXALLOC", "1024");

    QCoreApplication::setApplicationName(SOFTWARENAME);
    QCoreApplication::setApplicationVersion("1.0");

    // Load and apply the style sheet
    QFile file("");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        a.setStyleSheet(stream.readAll());
    }

    // Create a pixmap for the splash screen background
    QPixmap backgroundPixmap(":/Images/Resources/Images/splashscreen_bg.png");

    FrameFlowSplashScreen splash(backgroundPixmap);
    splash.setFixedSize(640, 360);
    splash.show();

    // Simulate some loading time
    QTimer::singleShot(5000, &splash, &QSplashScreen::close);

    MainWindow w;
    w.applySettings();
    w.setWindowTitle(SOFTWARENAME);
    w.showMaximized();
    return a.exec();
}
