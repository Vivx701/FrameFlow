#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QAbstractListModel>
#include <QImage>
#include <QFileInfo>

enum ImageRoles {

    ImagePathRole = Qt::UserRole + 1,
    PreviewRole,
    ResolutionRole,
    FileNameRole
};

struct ImageItem {
    QImage preview;
    QSize resolution;
    QFileInfo info;
};

class ImageModel : public QAbstractListModel
{
public:
    explicit ImageModel(QObject *parent = nullptr);

    // Add an image to the model
    void addImage(const QString& imagePath);

    // Remove an image from the model
    void removeImage(int index);

    // Change the order of images
    void moveImage(int fromIndex, int toIndex);

    ImageItem createImageItem(QString path);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    QList<QImage> getImageList();

private:

    QList<ImageItem> m_images;
};

#endif // IMAGEMODEL_H
