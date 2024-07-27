#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "Strings.h"

class AboutDialog : public QDialog
{
public:
    AboutDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle(Strings::ABOUT_DIALOG_TITLE);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel(Strings::APP_NAME);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold;");

        QLabel *versionLabel = new QLabel(Strings::APP_VERSION);
        versionLabel->setAlignment(Qt::AlignCenter);

        QLabel *descriptionLabel = new QLabel(Strings::APP_DESCRIPTION);
        descriptionLabel->setWordWrap(true);
        descriptionLabel->setAlignment(Qt::AlignCenter);

        QLabel *copyrightLabel = new QLabel(Strings::APP_COPYRIGHT);
        copyrightLabel->setAlignment(Qt::AlignCenter);

        QLabel *ffmpegLabel = new QLabel(Strings::FFMPEG_REFERENCE);
        ffmpegLabel->setAlignment(Qt::AlignCenter);

        QLabel *gifhLabel = new QLabel(Strings::GIFH_REFERENCE);
        gifhLabel->setAlignment(Qt::AlignCenter);

        QLabel *gifhLinkLabel = new QLabel(QString("<a href='%1'>%1</a>").arg(Strings::GIFH_LINK));
        gifhLinkLabel->setAlignment(Qt::AlignCenter);
        gifhLinkLabel->setOpenExternalLinks(true);

        QPushButton *qtVersionButton = new QPushButton(Strings::QT_VERSION_BUTTON_TEXT);
        connect(qtVersionButton, &QPushButton::clicked, this, &AboutDialog::showQtVersion);

        QPushButton *closeButton = new QPushButton(Strings::CLOSE_BUTTON_TEXT);
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
        QMessageBox::aboutQt(this, Strings::QT_VERSION_DIALOG_TITLE);
    }

};
#endif // ABOUTDIALOG_H
