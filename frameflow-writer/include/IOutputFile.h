#ifndef IOUTPUTFILE_H
#define IOUTPUTFILE_H
#include <QString>
#include <QList>
#include <QObject>
#include <frameflowexception.h>
#include <Types.h>
#include <QDebug>

#if defined(_WIN32) || defined(_WIN64)
#  ifdef FRAMEFLOW_WRITER_LIBRARY
#    define FRAMEFLOW_WRITER_EXPORT __declspec(dllexport)
#  else
#    define FRAMEFLOW_WRITER_EXPORT __declspec(dllimport)
#  endif
#else
#  define FRAMEFLOW_WRITER_EXPORT
#endif

class FRAMEFLOW_WRITER_EXPORT IOutputFile: public QObject {

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
    void saveStarted(QString name);
    void saveFinished(bool, QString name);
protected:
    ImageList m_Images;
    Attributes m_Attrib;
};



#endif // IOUTPUTFILE_H
