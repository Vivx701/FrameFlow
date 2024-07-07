#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QImage>
#include <Types.h>
#include <QColorSpace>

class ImageModel : public QAbstractListModel
{
    Q_OBJECT

public:

    explicit ImageModel(QObject *parent = nullptr);
    ~ImageModel();

    void addImage(const QString& imagePath);
    void removeImage(int index);
    void moveImage(int fromIndex, int toIndex);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QList<QImage> getImageList();
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QString getSelectedImageProperties(const QModelIndex& index);


private:
    QList<ImageItem> m_images;
    ImageItem createImageItem(QString path);
};

#endif // IMAGEMODEL_H
