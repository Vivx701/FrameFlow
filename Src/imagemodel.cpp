#include "imagemodel.h"


ImageModel::ImageModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void ImageModel::addImage(const QString& imagePath)
{
    ImageItem item = createImageItem(imagePath);
    if(item.info.exists())
    {
        beginInsertRows(QModelIndex(), m_images.size(), m_images.size());
        m_images.append(item);
        endInsertRows();
    }
}

void ImageModel::removeImage(int index)
{
    if (index >= 0 && index < m_images.size()) {
        beginRemoveRows(QModelIndex(), index, index);
        m_images.removeAt(index);
        endRemoveRows();
    }
}

void ImageModel::moveImage(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < m_images.size() &&
        toIndex >= 0 && toIndex < m_images.size()) {
        beginMoveRows(QModelIndex(), fromIndex, fromIndex, QModelIndex(), toIndex);
        m_images.move(fromIndex, toIndex);
        endMoveRows();
    }
}

ImageItem ImageModel::createImageItem(QString path)
{
    ImageItem item;
    item.info = QFileInfo(path);
    if(item.info.exists())
    {
        QImage image(item.info.absoluteFilePath());
        item.resolution = image.size();
        item.preview = image.scaled(100, 75);
    }
    return item;
}

int ImageModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0; // No nested items
    return m_images.size();
}

QVariant ImageModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const int row = index.row();
    if (row < 0 || row >= m_images.size())
        return QVariant();

    const ImageItem& item = m_images.at(row);
    switch (role) {
    case Qt::DisplayRole:
        return item.info.absoluteFilePath();
    case PreviewRole:
        return QVariant::fromValue(item.preview);
    case ResolutionRole:
        return QVariant::fromValue(item.resolution);
    case FileNameRole:
        return QVariant::fromValue(item.info.fileName());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ImageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ImagePathRole] = "imagePath";
    roles[PreviewRole] = "preview";
    roles[ResolutionRole] = "resolution";
    roles[FileNameRole] = "preview";
    return roles;
}

QList<QImage> ImageModel::getImageList()
{
    QList<QImage> images;
    for(ImageItem item:  m_images)
    {
        images.append(QImage(item.info.absoluteFilePath()));
    }
    return images;
}
