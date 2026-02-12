#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QAbstractListModel>
#include <QFileInfo>
#include <QImage>
#include <Types.h>
#include <QColorSpace>
#include <QVector>
#include <QPair>

#include <Types.h>

class ImageModel : public QAbstractListModel
{
    Q_OBJECT

public:

    explicit ImageModel(QObject *parent = nullptr);
    ~ImageModel();
    void addImage(const QString& imagePath);
    void removeImage(int index);
    void moveImage(int fromIndex, int toIndex);
    void clearImages();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QList<QImage> getImageList();
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVector<QPair<QString, QString>> getSelectedImageProperties(const QModelIndex& index);

signals:
    void imagesCleared();

private:
    QList<ImageItem> m_images;
    ImageItem createImageItem(QString path);

    // Property names
    QString FILENAME_PROPERTY = QObject::tr("Filename");
    QString DIR_PROPERTY = QObject::tr("Dir");
    QString BYTES_PROPERTY = QObject::tr("Bytes");
    QString MODIFIED_PROPERTY = QObject::tr("Modified");
    QString WIDTH_PROPERTY = QObject::tr("Width");
    QString HEIGHT_PROPERTY = QObject::tr("Height");
    QString DEPTH_PROPERTY = QObject::tr("Depth");
    QString FORMAT_PROPERTY = QObject::tr("Format");
    QString COLOR_COUNT_PROPERTY = QObject::tr("Color Count");
    QString IS_NULL_PROPERTY = QObject::tr("Is Null");
    QString BYTES_PER_LINE_PROPERTY = QObject::tr("Bytes Per Line");
    QString DEVICE_PIXEL_RATIO_PROPERTY = QObject::tr("Device Pixel Ratio");
    QString DOTS_PER_METER_X_PROPERTY = QObject::tr("DotsPerMeterX");
    QString DOTS_PER_METER_Y_PROPERTY = QObject::tr("DotsPerMeterY");
    QString OFFSET_PROPERTY = QObject::tr("Offset");
    QString IS_GRAYSCALE_PROPERTY = QObject::tr("Is Grayscale");
    QString HAS_ALPHA_CHANNEL_PROPERTY = QObject::tr("Has Alpha Channel");
    QString BIT_PLANE_COUNT_PROPERTY = QObject::tr("Bit Plane Count");
    QString INDEX_PROPERTY = QObject::tr("Index");

    // Unit strings
    QString PIXELS_UNIT = QObject::tr("px");
    QString BITS_UNIT = QObject::tr("bits");

    // Yes/No strings
    QString YES = QObject::tr("Yes");
    QString NO = QObject::tr("No");
};

#endif // IMAGEMODEL_H
