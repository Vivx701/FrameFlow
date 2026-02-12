#ifndef IMAGESPRITEATTRIBUTESDIALOG_H
#define IMAGESPRITEATTRIBUTESDIALOG_H

#include <QDialog>
#include <Types.h>

namespace Ui {
class ImageSpriteAttributesDialog;
}

class ImageSpriteAttributesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ImageSpriteAttributesDialog(ImageSpriteAttributes &attrs,
                                         QWidget *parent = nullptr);

    ~ImageSpriteAttributesDialog();

private:
    Ui::ImageSpriteAttributesDialog *ui;
    ImageSpriteAttributes &m_attrs;
    QString m_format;

private slots:
    void accept() override;
    void onBrowseClicked();
};


#endif // IMAGESPRITEATTRIBUTESDIALOG_H
