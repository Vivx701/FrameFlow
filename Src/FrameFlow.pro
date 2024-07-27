QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
    Include/Strings.h \
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

DISTFILES += \
    Resources/Theme/Dark.qss \
    Resources/Theme/Light.qss
