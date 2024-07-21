QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS +=  -lavformat -lavcodec -lavutil -lswscale

SOURCES += \
    exportdialog.cpp \
    giffile.cpp \
    htmlgallery.cpp \
    imagedelegate.cpp \
    imagemodel.cpp \
    imagesprite.cpp \
    main.cpp \
    mainwindow.cpp \
    outputfilefactory.cpp \
    pdffile.cpp \
    progressdialog.cpp \
    videofile.cpp

HEADERS += \
    IOutputFile.h \
    Strings.h \
    Types.h \
    exportdialog.h \
    giffile.h \
    htmlgallery.h \
    imagedelegate.h \
    imagemodel.h \
    imagesprite.h \
    mainwindow.h \
    outputfilefactory.h \
    pdffile.h \
    progressdialog.h \
    videofile.h\
    gif.h

FORMS += \
    exportdialog.ui \
    mainwindow.ui \
    progressdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
