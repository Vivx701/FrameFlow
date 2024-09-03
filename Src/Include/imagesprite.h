#ifndef IMAGESPRITE_H
#define IMAGESPRITE_H

#include <QImageWriter>
#include "IOutputFile.h"

class ImageSprite : public IOutputFile
{
public:
    explicit ImageSprite(QObject *parent = nullptr);
    bool save() override;
    QImage horizontalStitch(QColor bgColor=Qt::white);
    QImage verticalStitch(QColor bgColor=Qt::white);
};

#endif // IMAGESPRITE_H
