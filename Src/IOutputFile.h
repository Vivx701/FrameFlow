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

    virtual void save() = 0;

    void setAttrib(const Attributes &newAttrib)
    {
        m_Attrib = newAttrib;
    }

    Attributes Attrib() const
    {
        return m_Attrib;
    }

signals:
    void progressChanged(int maximum, int value);

protected:
    ImageList m_Images;
    Attributes m_Attrib;
};



#endif // IOUTPUTFILE_H
