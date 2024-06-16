#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("QT_IMAGEIO_MAXALLOC", "1024");
    MainWindow w;
    w.show();
    return a.exec();
}
