#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QFont>

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

    MainWindow w;
    w.applySettings();
    w.setWindowTitle(SOFTWARENAME);
    w.show();
    return a.exec();
}
