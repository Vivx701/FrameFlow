#ifndef FRAMEFLOWSPLASHSCREEN_H
#define FRAMEFLOWSPLASHSCREEN_H
#include <QSplashScreen>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

class FrameFlowSplashScreen : public QSplashScreen
{
public:
    FrameFlowSplashScreen(const QPixmap &pixmap) : QSplashScreen(pixmap)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Add app name label
        QLabel *nameLabel = new QLabel("FrameFlow", this);
        nameLabel->setStyleSheet("font-size: 24pt; font-weight: bold; color: white;");
        nameLabel->setAlignment(Qt::AlignCenter);

        // Add icon label
        QLabel *iconLabel = new QLabel(this);
        QPixmap icon(":/Icons/Resources/Icons/FrameFlow_500px.png");  // Replace with actual path
        iconLabel->setPixmap(icon.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        iconLabel->setAlignment(Qt::AlignCenter);

        // Add version label
        QLabel *versionLabel = new QLabel("Version 1.0", this);
        versionLabel->setStyleSheet("font-size: 12pt; color: white;");
        versionLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(iconLabel);
        layout->addWidget(nameLabel);
        layout->addWidget(versionLabel);

        setLayout(layout);
    }
};


#endif // FRAMEFLOWSPLASHSCREEN_H
