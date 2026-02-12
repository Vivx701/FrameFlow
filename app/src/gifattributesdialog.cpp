#include "gifattributesdialog.h"
#include "ui_gifattributesdialog.h"
#include <frameflowstrings.h>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

GifAttributesDialog::GifAttributesDialog(GifAttributes &attrs,
                                         QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GifAttributesDialog)
    , m_attrs(attrs)
{
    ui->setupUi(this);
    auto &s = m_attrs.specificSettings;
    ui->spinDelay->setValue(s.value("Delay").toInt());
    connect(ui->btnBrowse, &QPushButton::clicked,
            this, &GifAttributesDialog::onBrowseClicked);

}

GifAttributesDialog::~GifAttributesDialog()
{
    delete ui;
}

void GifAttributesDialog::onBrowseClicked()
{
    QString file = QFileDialog::getSaveFileName(
        this,
        GIF_EXPORT_DIALOG_TITLE,
        ui->editExportPath->text(),
        GIF_EXPORT_FILE_FILTER
        );

    if (file.isEmpty())
        return;

    // Ensure .gif extension
    if (!file.endsWith(".gif", Qt::CaseInsensitive))
        file += ".gif";

    ui->editExportPath->setText(file);
}


void GifAttributesDialog::accept()
{
    auto &s = m_attrs.specificSettings;

    const int delay = ui->spinDelay->value();
    const QString outFile = ui->editExportPath->text().trimmed();

    /* ---- Delay validation ---- */
    if (delay <= 0) {
        QMessageBox::warning(
            this,
            GIF_ATTRIBUTES_INVALID_TITLE,
            GIF_DELAY_INVALID_MSG
            );
        return;
    }

    /* ---- Export path validation ---- */
    if (outFile.isEmpty()) {
        QMessageBox::warning(
            this,
            GIF_ATTRIBUTES_INVALID_TITLE,
            GIF_EXPORT_PATH_MISSING_MSG
            );
        return;
    }

    QFileInfo fi(outFile);
    QDir dir = fi.dir();

    if (!dir.exists() || !dir.isReadable()) {
        QMessageBox::warning(
            this,
            GIF_ATTRIBUTES_INVALID_TITLE,
            GIF_EXPORT_PATH_INVALID_MSG
            );
        return;
    }

    /* ---- Write UI → struct ---- */
    s["Delay"]      = delay;
    m_attrs.filePath = outFile;

    QDialog::accept();
}

