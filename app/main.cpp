#include "frameflow.h"
#include "frameflowstrings.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(SOFTWARENAME);
    QCoreApplication::setApplicationVersion(VERSION);
    QCoreApplication::setOrganizationName(OWNERNAME);


    QSettings settings;
    QString lang = settings.value(SETTINGS_LANG, "en_US").toString();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FrameFlow_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    FrameFlow w;
    w.setWindowIconText(SOFTWARE_TITLE);
    w.setWindowTitle(SOFTWARE_TITLE);
    w.setWindowIcon(QIcon(":/Resources/Icons/FrameFlow.svg"));
    w.showMaximized();
    return a.exec();
}
