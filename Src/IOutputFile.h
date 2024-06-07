#ifndef IOUTPUTFILE_H
#define IOUTPUTFILE_H
#include <QString>
#include <QList>
#include <QImage>
#include <QObject>

typedef QList<QImage> ImageList;
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

    virtual void save(const QString& filename) = 0;

protected:
    ImageList m_Images;
};
#endif // IOUTPUTFILE_H
