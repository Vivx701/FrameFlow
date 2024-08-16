#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <Types.h>

class AboutDialog : public QDialog
{
public:
    AboutDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle(ABOUT_DIALOG_TITLE);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel(APP_NAME);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold;");

        QLabel *versionLabel = new QLabel(APP_VERSION);
        versionLabel->setAlignment(Qt::AlignCenter);

        QLabel *descriptionLabel = new QLabel(APP_DESCRIPTION);
        descriptionLabel->setWordWrap(true);
        descriptionLabel->setAlignment(Qt::AlignCenter);

        QLabel *copyrightLabel = new QLabel(APP_COPYRIGHT);
        copyrightLabel->setAlignment(Qt::AlignCenter);

        QLabel *ffmpegLabel = new QLabel(FFMPEG_REFERENCE);
        ffmpegLabel->setAlignment(Qt::AlignCenter);

        QLabel *gifhLabel = new QLabel(GIFH_REFERENCE);
        gifhLabel->setAlignment(Qt::AlignCenter);

        QLabel *gifhLinkLabel = new QLabel(QString("<a href='%1'>%1</a>").arg(GIFH_LINK));
        gifhLinkLabel->setAlignment(Qt::AlignCenter);
        gifhLinkLabel->setOpenExternalLinks(true);

        QPushButton *qtVersionButton = new QPushButton(QT_VERSION_BUTTON_TEXT);
        connect(qtVersionButton, &QPushButton::clicked, this, &AboutDialog::showQtVersion);

        QPushButton *closeButton = new QPushButton(CLOSE_BUTTON_TEXT);
        connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

        layout->addWidget(titleLabel);
        layout->addWidget(versionLabel);
        layout->addSpacing(10);
        layout->addWidget(descriptionLabel);
        layout->addSpacing(10);
        layout->addWidget(copyrightLabel);
        layout->addSpacing(20);
        layout->addWidget(ffmpegLabel);
        layout->addWidget(gifhLabel);
        layout->addWidget(gifhLinkLabel);
        layout->addSpacing(20);
        layout->addWidget(qtVersionButton);
        layout->addWidget(closeButton);

        setLayout(layout);
    }

private:
    void showQtVersion()
    {
        QString qtVersion = qVersion();
        QMessageBox::aboutQt(this, QT_VERSION_DIALOG_TITLE);
    }

    QString ABOUT_DIALOG_TITLE = QObject::tr("About FrameFlow");
    QString APP_NAME = QObject::tr(SOFTWARENAME);
    QString APP_VERSION = QObject::tr("Version")+" "+QString::number(VERSION);
    QString APP_DESCRIPTION = QObject::tr("FrameFlow is a versatile application designed to transform series of images into various multimedia formats. With FrameFlow, you can easily create videos, PDFs, sprite images, GIFs, and HTML presentations from your image sequences. Whether you're a designer, animator, or content creator, FrameFlow streamlines your workflow for efficient and creative visual storytelling.");
    QString APP_COPYRIGHT = QObject::tr("Copyright © 2024 vivx_developer (https://github.com/Vivx701/FrameFlow)");
    QString FFMPEG_REFERENCE = QObject::tr("This application uses FFmpeg for video processing.");
    QString GIFH_REFERENCE = QObject::tr("GIF encoding powered by gif-h");
    QString GIFH_LINK = QObject::tr("https://github.com/charlietangora/gif-h");
    QString QT_VERSION_BUTTON_TEXT = QObject::tr("Show Qt Version");
    QString CLOSE_BUTTON_TEXT = QObject::tr("Close");
    QString QT_VERSION_DIALOG_TITLE = QObject::tr("Qt Version");
    QString QT_VERSION_MESSAGE = QObject::tr("This application is using Qt version %1");

};
#endif // ABOUTDIALOG_H
