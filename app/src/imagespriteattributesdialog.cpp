#include "imagespriteattributesdialog.h"
#include "ui_imagespriteattributesdialog.h"
#include <frameflowstrings.h>
#include <QFileDialog>
#include <QMessageBox>

ImageSpriteAttributesDialog::ImageSpriteAttributesDialog(
        ImageSpriteAttributes &attrs, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImageSpriteAttributesDialog)
    , m_attrs(attrs)
{
    ui->setupUi(this);

    auto &s = m_attrs.specificSettings;

    ui->editCreator->setText(s.value("Creator").toString());
    ui->editAuthor->setText(s.value("Author").toString());

    ui->comboOrientation->setCurrentIndex(
        s.value("Orientation").toInt() == Qt::Horizontal ? 0 : 1);


    connect(ui->btnBrowse, &QPushButton::clicked,
            this, &ImageSpriteAttributesDialog::onBrowseClicked);
}


ImageSpriteAttributesDialog::~ImageSpriteAttributesDialog()
{
    delete ui;
}

void ImageSpriteAttributesDialog::onBrowseClicked()
{
    QString selectedFilter;

    QString file = QFileDialog::getSaveFileName(this, SPRITE_EXPORT_DIALOG_TITLE, ui->editExportPath->text(),
                                                SPRITE_EXPORT_FILTER_PNG+";;"+SPRITE_EXPORT_FILTER_JPG+";;"+SPRITE_EXPORT_FILTER_BMP, &selectedFilter );

    if (file.isEmpty())
        return;

    /* ---- Decide format from filter ---- */
    if (selectedFilter == SPRITE_EXPORT_FILTER_PNG) {
        m_format = "PNG";
    } else if (selectedFilter == SPRITE_EXPORT_FILTER_JPG) {
        m_format = "JPG";
    } else if (selectedFilter == SPRITE_EXPORT_FILTER_BMP) {
        m_format = "BMP";
    } else {
        m_format = "PNG";
    }

    /* ---- Enforce correct extension ---- */
    QFileInfo fi(file);
    if (fi.suffix().isEmpty()) {
        file += "." + m_format.toLower();
    }

    ui->editExportPath->setText(file);
}


void ImageSpriteAttributesDialog::accept()
{
    auto &s = m_attrs.specificSettings;
    const QString outFile = ui->editExportPath->text().trimmed();

    if (outFile.isEmpty()) {
        QMessageBox::warning(
            this,
            SPRITE_ATTRIBUTES_INVALID_TITLE,
            SPRITE_EXPORT_PATH_MISSING_MSG
        );
        return;
    }

    QFileInfo fi(outFile);
    QDir dir = fi.dir();
    if (!dir.exists() || !dir.isReadable()) {
        QMessageBox::warning(
            this,
            SPRITE_ATTRIBUTES_INVALID_TITLE,
            SPRITE_EXPORT_PATH_INVALID_MSG
        );
        return;
    }

    s["Creator"]     = ui->editCreator->text();
    s["Author"]      = ui->editAuthor->text();
    s["Format"]      = "";
    s["Orientation"] = (ui->comboOrientation->currentIndex() == 0)
                        ? Qt::Horizontal
                        : Qt::Vertical;
    m_attrs.filePath = outFile;
    QDialog::accept();
}
