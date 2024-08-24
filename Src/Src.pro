QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += Include
LIBS +=  -lavformat -lavcodec -lavutil -lswscale

SOURCES += \
    Source/exportdialog.cpp \
    Source/giffile.cpp \
    Source/htmlgallery.cpp \
    Source/iconthememanager.cpp \
    Source/imagedelegate.cpp \
    Source/imagemodel.cpp \
    Source/imagesprite.cpp \
    main.cpp \
    Source/mainwindow.cpp \
    Source/outputfilefactory.cpp \
    Source/pdffile.cpp \
    Source/progressdialog.cpp \
    Source/settingsdialog.cpp \
    Source/videofile.cpp

HEADERS += \
    Include/FrameFlowSplashScreen.h \
    Include/IOutputFile.h \
    Include/Types.h \
    Include/aboutDialog.h \
    Include/exportdialog.h \
    Include/giffile.h \
    Include/htmlgallery.h \
    Include/iconthememanager.h \
    Include/imagedelegate.h \
    Include/imagemodel.h \
    Include/imagesprite.h \
    Include/mainwindow.h \
    Include/outputfilefactory.h \
    Include/pdffile.h \
    Include/progressdialog.h \
    Include/settingsdialog.h \
    Include/videofile.h\
    Include/gif.h

FORMS += \
    Forms/exportdialog.ui \
    Forms/mainwindow.ui \
    Forms/progressdialog.ui \
    Forms/settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

TRANSLATIONS = \
    Translations/frameflow_en.ts \
    Translations/frameflow_ml.ts \

DISTFILES += \
    Resources/Theme/Dark.qss \
    Resources/Theme/Light.qss \
    Resources/Theme/Default.qss

# Specify the output directory
DESTDIR = $$OUT_PWD/../bin

# Specify where to put object files
OBJECTS_DIR = $$OUT_PWD/../build/obj

# Specify where to put moc files
MOC_DIR = $$OUT_PWD/../build/moc

# Specify where to put ui files
UI_DIR = $$OUT_PWD/../build/ui

# Specify where to put rcc files
RCC_DIR = $$OUT_PWD/../build/rcc
