#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(ImageList& images, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog),
    m_images(images)
{
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ui->setupUi(this);
    ui->closeButton->setEnabled(false);
}

void ProgressDialog::setFilename(QString path)
{
    ui->filename->setText(SAVING_MESSAGE+": "+path);
}

void ProgressDialog::setAttributes(Attributes &attrib, OutputType type)
{
    rThread = new RenderingThread(m_images, attrib, type, this);
    connect(rThread, &QThread::finished, [this](){
        #ifdef UNIT_TESTING
                this->accept();
        #endif

    });

    connect(rThread, &RenderingThread::success, [this](bool result){

        ui->filename->setText(result? SAVING_SUCCESS_MESSAGE: SAVING_FAILED_MESSAGE);
        ui->closeButton->setEnabled(true);
    });

}

ProgressDialog::~ProgressDialog()
{
    if(rThread)
    {
        if(rThread->isRunning())
        {
            rThread->terminate();
        }
        rThread->deleteLater();
    }
    delete ui;
}

void ProgressDialog::onProgressChanged(int max, int value)
{
    if(ui->progressBar->maximum() != max)
    {
        ui->progressBar->setMaximum(max);
    }

    ui->progressBar->setValue(value);
}

ImageList &ProgressDialog::images() const
{
    return m_images;
}

int ProgressDialog::exec()
{
    if(rThread)
    {
        connect(rThread, &RenderingThread::progressChanged, this, &ProgressDialog::onProgressChanged);
        rThread->start();
    }
    return QDialog::exec();
}


