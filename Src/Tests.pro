include(FrameFlow.pro)
DEFINES += UNIT_TESTING
QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase
TEMPLATE = app
SOURCES +=  Tests/tst_frameflow.cpp

DISTFILES += \
    Tests/Files/Red.jpg \
    Tests/Files/Green.jpg \
    Tests/Files/Blue.jpg \
    Tests/Files/Multicolor.jpg

RESOURCES += \
    TestingResources.qrc
