#ifndef TYPES_H
#define TYPES_H

#include <QColor>
#include <QString>
#include <QVariantMap>
#include <QImage>
#include <QPageSize>
#include <QDateTime>
#include <QMarginsF>
#include <QPageLayout>
#include <QPagedPaintDevice>


#define SOFTWARENAME "FrameFlow"
#define VERSION 1.0

struct Attributes
{
    QColor background;
    QString filePath;
    QVariantMap specificSettings;
};

struct PDFAttributes: public Attributes
{
    PDFAttributes()
    {
        background = QColor::fromRgb(255, 255, 255);
        specificSettings = {
                                {"Size", QPageSize::A4},
                                {"Creator", SOFTWARENAME},
                                {"Margin",  QVariant::fromValue(QMarginsF(10, 10, 10, 10))},
                                {"Orientation", QPageLayout::Portrait},
                                {"Version", QVariant::fromValue(QPagedPaintDevice::PdfVersion_1_4)},
                                {"Title", ""}

                            };

    }

    PDFAttributes(const PDFAttributes &copy) = default;
};

struct ImageSpriteAttributes: public Attributes
{
    ImageSpriteAttributes()
    {
        background = QColor::fromRgb(255, 255, 255);
        specificSettings = {
            {"Creator", SOFTWARENAME},
            {"Format",  "PNG"},
            {"Orientation", Qt::Horizontal},
            {"Author", SOFTWARENAME}
        };
    }

    ImageSpriteAttributes(const ImageSpriteAttributes &copy) = default;
};

struct GifAttributes: public Attributes
{
    GifAttributes()
    {
        background = QColor::fromRgb(255, 255, 255);
        specificSettings = {
            {"Creator", SOFTWARENAME},
            {"Author", SOFTWARENAME},
            {"FPS",  10}
        };
    }

    GifAttributes(const GifAttributes &copy) = default;
};

typedef QList<QImage> ImageList;

#endif // TYPES_H
