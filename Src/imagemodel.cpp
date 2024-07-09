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
QString ImageModel::getSelectedImageProperties(const QModelIndex& index)
{
    if(!index.isValid())
    {
        return QString();
    }

    const ImageItem& item = m_images.at(index.row());

    QImage image(item.info.absoluteFilePath());
    QString properties;
    properties += "Filename: " + item.info.fileName() + "\n";
    properties += "Dir: " + item.info.absolutePath() + "\n";
    properties += "Bytes: " + QString::number(item.info.size()) + "\n\n\n";

    properties += "Width: " + QString::number(image.width()) + " px\n";
    properties += "Height: " + QString::number(image.height()) + " px\n";
    properties += "Depth: " + QString::number(image.depth()) + " bits\n";
    properties += "Format: " + QString::number(image.format()) + "\n";
    properties += "Color Count: " + QString::number(image.colorCount()) + "\n";
    properties += "Is Null: " + QString(image.isNull() ? "Yes" : "No") + "\n";
    properties += "Bytes Per Line: " + QString::number(image.bytesPerLine()) + "\n";
    properties += "Device Pixel Ratio: " + QString::number(image.devicePixelRatio()) + "\n";
    properties += "DotsPerMeterX: " + QString::number(image.dotsPerMeterX()) + "\n";
    properties += "DotsPerMeterY: " + QString::number(image.dotsPerMeterY()) + "\n";
    properties += "Offset: (" + QString::number(image.offset().x()) + ", " + QString::number(image.offset().y()) + ")\n";
    properties += "Is Grayscale: " + QString(image.isGrayscale() ? "Yes" : "No") + "\n";
    properties += "Has Alpha Channel: " + QString(image.hasAlphaChannel() ? "Yes" : "No") + "\n";
    properties += "Bit Plane Count: " + QString::number(image.bitPlaneCount()) + "\n";

    return properties;
}


