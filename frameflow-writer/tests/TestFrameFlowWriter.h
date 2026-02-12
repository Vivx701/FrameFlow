#ifndef TESTFRAMEFLOWWRITER_H   
#define TESTFRAMEFLOWWRITER_H

#include <QObject>
#include <QtTest/QtTest>
#include <frameflowwriter.h>

class FrameFlowWriterTest : public QObject
{
    Q_OBJECT

private slots:
    void construct_destroy();
    void createImageList();
    void createVideo();
    void createGif();
    void createSprite();
    void cleanup();
    
};

 
#endif // TESTFRAMEFLOWWRITER_H