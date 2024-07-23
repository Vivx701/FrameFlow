#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("QT_IMAGEIO_MAXALLOC", "1024");

    QCoreApplication::setApplicationName(SOFTWARENAME);
    QCoreApplication::setApplicationVersion("1.0");
    // Load and apply the style sheet
    QFile file(":/Theme/Resources/Theme/Default.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        a.setStyleSheet(stream.readAll());
    }

    MainWindow w;
    w.setWindowTitle(SOFTWARENAME);
    w.show();
    return a.exec();
}
