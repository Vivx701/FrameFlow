#include "TestFrameFlowWriter.h"

#include <QtTest/QtTest>
#include <QByteArray>
#include <QFileInfo>

VideoAttributes vAttrs;
GifAttributes gAttrs;
ImageSpriteAttributes sAttrs;

void FrameFlowWriterTest::cleanup()
{
    //QFile::remove(vAttrs.filePath);
    //QFile::remove(gAttrs.filePath);
    //QFile::remove(sAttrs.filePath);
}

void FrameFlowWriterTest::construct_destroy()
{
    FrameFlowWriter writer;
    QVERIFY(true);
}

void FrameFlowWriterTest::createImageList()
{
    FrameFlowWriter writer;
    ImageList images;
    images.append(QImage("SampleImages/red.jpg"));
    images.append(QImage("SampleImages/green.jpg"));
    images.append(QImage("SampleImages/blue.jpg"));
    QVERIFY(images.count() == 3);
}

void FrameFlowWriterTest::createVideo()
{
    FrameFlowWriter writer;
    auto videoWriter = writer.createOutputFile(OutputType::VIDEO);
    QVERIFY(videoWriter != nullptr);
    ImageList images;
    images.append(QImage("SampleImages/red.jpg"));
    images.append(QImage("SampleImages/green.jpg"));
    images.append(QImage("SampleImages/blue.jpg"));
    videoWriter->addImages(images); 

    vAttrs.filePath = "test_output.mp4";
    vAttrs.specificSettings["Creator"] = "UnitTest";
    vAttrs.specificSettings["Author"] = "UnitTest";
    vAttrs.specificSettings["Title"] = "Test Video";
    vAttrs.specificSettings["Description"] = "This is a test video created during unit testing.";
    vAttrs.specificSettings["FPS"] = 24;
    vAttrs.specificSettings["FrameDelay"] = 1000; // 1 second per
    videoWriter->setAttrib(vAttrs);
    bool result = videoWriter->save();
    QVERIFY(result);
    QFileInfo outputFile(vAttrs.filePath);
    QVERIFY2(outputFile.exists(), "Output file does not exist.");
    QVERIFY2(outputFile.size() > 0, "Output file is empty.");
    QVERIFY2(outputFile.isFile(), "Output file is not a regular file.");
}


void FrameFlowWriterTest::createGif()
{
    FrameFlowWriter writer;
    auto gifWriter = writer.createOutputFile(OutputType::GIF);
    QVERIFY(gifWriter != nullptr);
    ImageList images;
    images.append(QImage("SampleImages/red.jpg"));
    images.append(QImage("SampleImages/green.jpg"));
    images.append(QImage("SampleImages/blue.jpg"));
    gifWriter->addImages(images); 

    gAttrs.filePath = "test_output.gif";
    gAttrs.specificSettings["Delay"] = 150; // 150 ms per frame
    gifWriter->setAttrib(gAttrs);
    bool result = gifWriter->save();
    QVERIFY(result);
    QFileInfo outputFile(gAttrs.filePath);
    QVERIFY2(outputFile.exists(), "Output file does not exist.");
    QVERIFY2(outputFile.size() > 0, "Output file is empty.");
    QVERIFY2(outputFile.isFile(), "Output file is not a regular file.");
}

void FrameFlowWriterTest::createSprite()
{
    FrameFlowWriter writer;
    auto spriteWriter = writer.createOutputFile(OutputType::SPRITE);
    QVERIFY(spriteWriter != nullptr);
    ImageList images;
    images.append(QImage("SampleImages/red.jpg"));
    images.append(QImage("SampleImages/green.jpg"));
    images.append(QImage("SampleImages/blue.jpg"));
    spriteWriter->addImages(images); 

    sAttrs.filePath = "test_output_sprite.png";
    spriteWriter->setAttrib(sAttrs);
    bool result = spriteWriter->save();
    QVERIFY(result);
    QFileInfo outputFile(sAttrs.filePath);
    QVERIFY2(outputFile.exists(), "Output file does not exist.");       
    QVERIFY2(outputFile.size() > 0, "Output file is empty.");
    QVERIFY2(outputFile.isFile(), "Output file is not a regular file.");
}
QTEST_MAIN(FrameFlowWriterTest);