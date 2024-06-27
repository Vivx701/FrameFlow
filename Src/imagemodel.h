#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QAbstractListModel>
#include <QImage>
#include <QFileInfo>

// Custom roles for the image model
enum ImageRoles {
    ImagePathRole = Qt::UserRole + 1,  // Role for storing the image path
    PreviewRole,                       // Role for storing the image preview
    ResolutionRole,                    // Role for storing the image resolution
    FileNameRole                       // Role for storing the image file name
};

// Struct representing an image item in the model
struct ImageItem {
    QImage preview;     // Image preview
    QSize resolution;   // Image resolution
    QFileInfo info;     // Image file information
};

// Image model class inheriting from QAbstractListModel
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

    // Create an ImageItem object from the provided image path
    ImageItem createImageItem(QString path);

    // Return the number of rows (images) in the model
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    // Return the data for a specific index and role in the model
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Return a hash map of role names used by the model
    QHash<int, QByteArray> roleNames() const override;

    // Return a list of all images stored in the model
    QList<QImage> getImageList();

private:
    // List of ImageItem objects storing the image data
    QList<ImageItem> m_images;
};

#endif // IMAGEMODEL_H
