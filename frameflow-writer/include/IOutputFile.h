#ifndef IOUTPUTFILE_H
#define IOUTPUTFILE_H
#include <QString>
#include <QList>
#include <QObject>

#include <Types.h>
#include <QDebug>

class IOutputFile: public QObject {

    Q_OBJECT

public:
    virtual ~IOutputFile() = default;
    IOutputFile(QObject *parent=nullptr): QObject(parent)
    {
    }

    void addImages(ImageList &images)
    {
        m_Images.append(images);
    }
    void clearImages()
    {
        m_Images.clear();
    }

    virtual bool save() = 0;

    void setAttrib(const Attributes &newAttrib)
    {
        m_Attrib = newAttrib;
    }

    Attributes Attrib() const
    {
        return m_Attrib;
    }

    QSize getMaxSize()
    {
        int height = 0, width = 0;
        for(QImage img:  m_Images)
        {
            height = qMax(height, img.height());
            width  = qMax(width, img.width());
        }
        return QSize(width, height);
    }

signals:
    void progressChanged(int maximum, int value);

protected:
    ImageList m_Images;
    Attributes m_Attrib;
};



#endif // IOUTPUTFILE_H
