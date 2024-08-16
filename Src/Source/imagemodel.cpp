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
        beginRemoveRows(QModelIndex(), index, index);
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
    if(fromIndex == toIndex)
    {
        return;
    }
    if (fromIndex >= 0 && fromIndex < m_images.size() &&
            toIndex >= 0 && toIndex < m_images.size()) {
        beginMoveRows(QModelIndex(), fromIndex, fromIndex, QModelIndex(), toIndex > fromIndex ? toIndex + 1 : toIndex);
        m_images.move(fromIndex, toIndex);
        endMoveRows();
    }
}

/**
 * @brief Clears all images from the model.
 *
 * This function removes all image items from the model's internal storage.
 * It uses the model reset functions to ensure all connected views are properly updated.
 * After calling this function, the model will be empty.
 *
 * @note This operation cannot be undone. All image data in the model will be lost.
 *
 * @see beginResetModel(), endResetModel()
 *
 * @emit imagesCleared() Signal emitted after all images have been removed.
 */
void ImageModel::clearImages()
{
    beginResetModel();
    m_images.clear();
    endResetModel();
    emit imagesCleared();
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

/**
 * @brief Generates a string containing all properties of a QImage.
 *
 * This function extracts various properties of a QImage and formats them
 * into a human-readable string, with each property labeled.
 *
 */
QVector<QPair<QString, QString>> ImageModel::getSelectedImageProperties(const QModelIndex& index)
{
    QVector<QPair<QString, QString>> properties;

    if(!index.isValid())
    {
        return properties;
    }


    const ImageItem& item = m_images.at(index.row());

    QImage image(item.info.absoluteFilePath());
    properties.push_back(qMakePair(FILENAME_PROPERTY, item.info.fileName()));
    properties.push_back(qMakePair(DIR_PROPERTY, item.info.absolutePath()));
    properties.push_back(qMakePair(BYTES_PROPERTY, QString::number(item.info.size())));
    properties.push_back(qMakePair(MODIFIED_PROPERTY, item.info.lastModified().toString()));

    properties.push_back(qMakePair(WIDTH_PROPERTY, QString::number(image.width()) + " " + PIXELS_UNIT));
    properties.push_back(qMakePair(HEIGHT_PROPERTY, QString::number(image.height()) + " " + PIXELS_UNIT));
    properties.push_back(qMakePair(DEPTH_PROPERTY, QString::number(image.depth()) + " " + BITS_UNIT));
    properties.push_back(qMakePair(FORMAT_PROPERTY, QString::number(image.format())));
    properties.push_back(qMakePair(COLOR_COUNT_PROPERTY, QString::number(image.colorCount())));
    properties.push_back(qMakePair(IS_NULL_PROPERTY, image.isNull() ? YES : NO));
    properties.push_back(qMakePair(BYTES_PER_LINE_PROPERTY, QString::number(image.bytesPerLine())));
    properties.push_back(qMakePair(DEVICE_PIXEL_RATIO_PROPERTY, QString::number(image.devicePixelRatio())));
    properties.push_back(qMakePair(DOTS_PER_METER_X_PROPERTY, QString::number(image.dotsPerMeterX())));
    properties.push_back(qMakePair(DOTS_PER_METER_Y_PROPERTY, QString::number(image.dotsPerMeterY())));
    properties.push_back(qMakePair(OFFSET_PROPERTY, "(" + QString::number(image.offset().x()) + ", " + QString::number(image.offset().y()) + ")"));
    properties.push_back(qMakePair(IS_GRAYSCALE_PROPERTY, image.isGrayscale() ? YES : NO));
    properties.push_back(qMakePair(HAS_ALPHA_CHANNEL_PROPERTY, image.hasAlphaChannel() ? YES : NO));
    properties.push_back(qMakePair(BIT_PLANE_COUNT_PROPERTY, QString::number(image.bitPlaneCount())));
    properties.push_back(qMakePair(INDEX_PROPERTY, QString::number(index.row()+1)+"/"+QString::number(rowCount())));
    return properties;
}

