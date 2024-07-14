#include "exportdialog.h"
#include "ui_exportdialog.h"
#include <QFileDialog>
#include <QDir>

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    connect(ui->pdfBrowseButton, SIGNAL(clicked()), this, SLOT(openFolderDialog()));
    connect(ui->spriteBrowseButton, SIGNAL(clicked()), this, SLOT(openFolderDialog()));
    connect(ui->videoBrowseButton, SIGNAL(clicked()), this, SLOT(openFolderDialog()));
    connect(ui->htmlBrowseButton, SIGNAL(clicked()), this, SLOT(openFolderDialog()));


    attributeMap["pdf"] = [this](){
                                    PDFAttributes pattr;
                                    pattr.filePath = currentSavePath();
                                    pattr.specificSettings["Title"] = ui->pdfDocumentTitle->text();
                                    pattr.specificSettings["Fill"] = ui->pdfFill->currentText();
                                    pattr.specificSettings["Orientation"] = (ui->pdfOrientation->currentIndex() == 0)? QPageLayout::Portrait : QPageLayout::Landscape;
                                    pattr.specificSettings["Version"] = QVariant::fromValue(QPagedPaintDevice::PdfVersion_1_6);
                                    int margin = ui->pdfMargin->value();
                                    pattr.specificSettings["Margin"] = QVariant::fromValue(QMarginsF(margin, margin, margin, margin));
                                    return pattr;

                              };



}

QString ExportDialog::currentTabName()
{
    int currentIndex = ui->tabWidget->currentIndex();
    QWidget *currentTab = ui->tabWidget->widget(currentIndex);
    return currentTab->objectName();
}

QString ExportDialog::currentSavePath()
{
    QString filePath;
    QString tabName = currentTabName();
    QLineEdit *saveLocation = ui->tabWidget->findChild<QLineEdit*>(tabName+"SaveLocation");
    if(!saveLocation)
    {
        qDebug()<<"Save location control not found with name "<<tabName+"SaveLocation";
        return filePath;
    }
    QLineEdit *fileName = ui->tabWidget->findChild<QLineEdit*>(tabName+"FileName");
    if(!fileName)
    {
        qDebug()<<"Filename location control not found with name "<<tabName+"FileName";
        return filePath;
    }

    filePath =  QDir(saveLocation->text()).filePath(fileName->text());

    QString extension = ".";
    if(tabName == "pdf")
    {
        extension += "pdf";
    }
    else if(tabName == "sprite")
    {
        extension +=  ui->spriteFormatComboBox->currentText().toLower();
    }
    else if(tabName == "video")
    {
        extension +=  ".mp4";
    }
    else if(tabName == "html")
    {
        extension =  "";
    }

    return filePath+extension;
}

Attributes ExportDialog::exportSettings()
{

}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::openFolderDialog()
{
    QString folderPath = QFileDialog::getExistingDirectory(
                this,
                tr("Select Folder"), QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );

    if (!folderPath.isEmpty()) {
        // User selected a folder

        QLineEdit *saveLocation = ui->tabWidget->findChild<QLineEdit*>(currentTabName()+"SaveLocation");
        if(!saveLocation)
        {
            qDebug()<<"Save location control not found with name "<<currentTabName()+"SaveLocation";
            return;
        }
        saveLocation->setText(folderPath);
    }
}


