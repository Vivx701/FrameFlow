#include "imagemodel.h"

/**
 * @brief Constructor for the ImageModel class.
 * @param parent The parent object.
 */
ImageModel::ImageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

ImageModel::~ImageModel()
{
}

/**
 * @brief Add an image to the model using the provided image path.
 * @param imagePath The path of the image to be added.
 */
void ImageModel::addImage(const QString& imagePath)
{
    ImageItem item = createImageItem(imagePath);
    if(item.info.exists())
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_images.append(item);
        endInsertRows();
    }
}

/**
 * @brief Remove an image from the model at the specified index.
 * @param index The index of the image to be removed.
 */
void ImageModel::removeImage(int index)
{
    if (index >= 0 && index < m_images.size()) {
        beginRemoveRows(QModelIndex(), index / 3, index / 3);
        m_images.removeAt(index);
        endRemoveRows();
    }
}

/**
 * @brief Change the order of images by moving an image from one index to another.
 * @param fromIndex The index of the image to be moved.
 * @param toIndex The index where the image should be moved to.
 */
void ImageModel::moveImage(int fromIndex, int toIndex)
{
    if (fromIndex >= 0 && fromIndex < m_images.size() &&
        toIndex >= 0 && toIndex < m_images.size()) {
        beginMoveRows(QModelIndex(), fromIndex / 3, fromIndex / 3, QModelIndex(), toIndex / 3);
        m_images.move(fromIndex, toIndex);
        endMoveRows();
    }
}

/**
 * @brief Create an ImageItem object from the provided image path.
 * @param path The path of the image.
 * @return The created ImageItem object.
 */
ImageItem ImageModel::createImageItem(QString path)
{
    ImageItem item;
    item.info = QFileInfo(path);
    if(item.info.exists())
    {
        QImage image(item.info.absoluteFilePath());
        item.resolution = image.size();
        item.preview = image.scaled(500, 500);
    }
    return item;
}

/**
 * @brief Return the number of rows in the model.
 * @param parent The parent index (unused).
 * @return The number of rows in the model.
 */
int ImageModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0; // No nested items
    return m_images.size();
}

/**
 * @brief Return the number of columns in the model.
 * @param parent The parent index (unused).
 * @return The number of columns in the model.
 */
int ImageModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0; // No nested items
    return 1;
}

/**
 * @brief Return the data for a specific index and role in the model.
 * @param index The index of the item.
 * @param role The role of the data to be returned.
 * @return The data for the specified index and role.
 */
QVariant ImageModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int imageIndex = index.row();
    if (imageIndex < 0 || imageIndex >= m_images.size())
        return QVariant();
    const ImageItem& item = m_images.at(imageIndex);
    switch (role) {
    case Qt::DisplayRole:
        return item.info.absoluteFilePath();
    case Qt::SizeHintRole:
        return QSize(200, 200);
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

/**
 * @brief Return a hash map of role names used by the model.
 * @return The hash map of role names.
 */
QHash<int, QByteArray> ImageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ImagePathRole] = "imagePath";
    roles[PreviewRole] = "preview";
    roles[ResolutionRole] = "resolution";
    roles[FileNameRole] = "preview";
    return roles;
}

/**
 * @brief Return a list of all images stored in the model.
 * @return The list of images.
 */
QList<QImage> ImageModel::getImageList()
{
    QList<QImage> images;
    for(ImageItem item:  m_images)
    {
        images.append(QImage(item.info.absoluteFilePath()));
    }
    return images;
}

/**
 * @brief Create an index for the given row and column.
 * @param row The row number.
 * @param column The column number.
 * @param parent The parent index (unused in this model).
 * @return The model index for the specified row and column.
 */
QModelIndex ImageModel::index(int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid() || row < 0 || column < 0 || row >= rowCount() || column >= columnCount())
        return QModelIndex();

    return createIndex(row, column);
}

/**
 * @brief Return the item flags for the given model index.
 * @param index The model index.
 * @return The item flags for the index.
 */
Qt::ItemFlags ImageModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


