#include "videoattributesdialog.h"
#include "ui_videoattributesdialog.h"
#include "frameflowstrings.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>

VideoAttributesDialog::VideoAttributesDialog(VideoAttributes &attrs,
                                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoAttributesDialog)
    , m_attrs(attrs)
{
    ui->setupUi(this);

    auto &s = m_attrs.specificSettings;

    ui->editCreator->setText(s.value("Creator").toString());
    ui->editAuthor->setText(s.value("Author").toString());
    ui->spinFps->setValue(s.value("FPS").toInt());
    ui->spinFrameDelay->setValue(s.value("FrameDelay").toInt());
    ui->editTitle->setText(s.value("Title").toString());
    ui->editDescription->setPlainText(s.value("Description").toString());
    ui->editCopyright->setText(s.value("Copyright").toString());

    ui->spinFps->setMinimum(1);
    ui->spinFrameDelay->setMinimum(1);
    connect(ui->btnBrowse, &QPushButton::clicked, this, &VideoAttributesDialog::onBrowseClicked);

}

VideoAttributesDialog::~VideoAttributesDialog()
{
    delete ui;
}

void VideoAttributesDialog::accept()
{
    auto &s = m_attrs.specificSettings;

    const int fps        = ui->spinFps->value();
    const int frameDelay = ui->spinFrameDelay->value();
    const QString outFile = ui->editExportPath->text().trimmed();

    /* ---- FPS validation ---- */
    if (fps <= 0) {
        QMessageBox::warning(
            this,
            VIDEO_ATTRIBUTES_INVALID_TITLE,
            VIDEO_FPS_INVALID_MSG
            );
        return;
    }

    /* ---- FrameDelay validation ---- */
    if (frameDelay <= 0) {
        QMessageBox::warning(
            this,
            VIDEO_ATTRIBUTES_INVALID_TITLE,
            VIDEO_FRAME_DELAY_INVALID_MSG
            );
        return;
    }

    /* ---- Export path validation ---- */
    if (outFile.isEmpty()) {
        QMessageBox::warning(
            this,
            VIDEO_ATTRIBUTES_INVALID_TITLE,
            VIDEO_EXPORT_PATH_MISSING_MSG
            );
        return;
    }

    QFileInfo fi(outFile);
    QDir dir = fi.dir();

    if (!dir.exists() || !dir.isReadable()) {
        QMessageBox::warning(this, VIDEO_ATTRIBUTES_INVALID_TITLE, VIDEO_EXPORT_PATH_INVALID_MSG);
        return;
    }
    if (fi.suffix().isEmpty())
    {
        m_attrs.filePath = outFile + ".mp4";
    }
    else
    {
        m_attrs.filePath = outFile;
    }

    /* ---- Write UI → struct ---- */
    s["Creator"]     = ui->editCreator->text();
    s["Author"]      = ui->editAuthor->text();
    s["FPS"]         = fps;
    s["FrameDelay"]  = frameDelay;
    s["Title"]       = ui->editTitle->text();
    s["Description"] = ui->editDescription->toPlainText();
    s["Copyright"]   = ui->editCopyright->text();

    QDialog::accept();
}

void VideoAttributesDialog::onBrowseClicked()
{
    QString file = QFileDialog::getSaveFileName(
        this,
        VIDEO_EXPORT_DIALOG_TITLE,
        ui->editExportPath->text(),
        VIDEO_EXPORT_FILE_FILTER
        );

    if (file.isEmpty())
        return;

    QFileInfo fi(file);
    if (fi.suffix().isEmpty()) {
        file += ".mp4";
    }

    ui->editExportPath->setText(file);
}
