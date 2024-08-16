#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QFont>
#include <QTimer>
#include <QTranslator>
#include <QLocale>
#include <Types.h>
#include <FrameFlowSplashScreen.h>
#include "mainwindow.h"


#ifndef UNIT_TESTING
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("QT_IMAGEIO_MAXALLOC", "1024");

    QCoreApplication::setApplicationName(SOFTWARENAME);
    QCoreApplication::setApplicationVersion(QString::number(VERSION));

    QSettings settings;
    QString lang = settings.value(LANGUAGE, "en_US").toString();
    QLocale locale(lang);
    QString langFile = langMap[locale.language()].first;
    QTranslator  translator;
    // Load the translation file
    if (translator.load(langFile))
    {
        qApp->setProperty(LANGUAGE.toLocal8Bit().data(), lang);
        qApp->installTranslator(&translator);
    }
    // Create a pixmap for the splash screen background
    QPixmap backgroundPixmap(":/Images/Resources/Images/splashscreen_bg.png");

    FrameFlowSplashScreen splash(backgroundPixmap);
    splash.setFixedSize(640, 360);
    splash.show();

    // Simulate some loading time
    QTimer::singleShot(3000, &splash, &QSplashScreen::close);

    MainWindow w;
    w.applySettings();
    w.setWindowTitle(QObject::tr(SOFTWARENAME));
    w.showMaximized();
    return a.exec();
}
#endif
