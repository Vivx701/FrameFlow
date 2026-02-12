#include "saveprogressdialog.h"
#include "ui_saveprogressdialog.h"
#include <QTimer>
#include <frameflowstrings.h>
#include <QMessageBox>
#include <QtConcurrent>
#include <QPushButton>

SaveProgressDialog::SaveProgressDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SaveProgressDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowTitle("Writing File");
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(400, 140);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SaveProgressDialog::onAbortButtonPressed);
}

SaveProgressDialog::~SaveProgressDialog()
{
    delete ui;
}

void SaveProgressDialog::onSaveStarted(const QString &fileName)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setVisible(false);
    ui->labelStatus->setText(EXPORT_STARTED +" : "+fileName);
    saveFilePath = fileName;
    ui->progressBar->setValue(0);
}

void SaveProgressDialog::onProgressChanged(int total, int value)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(value);
}

void SaveProgressDialog::onSaveFinished(bool success, const QString &fileName)
{
    QString message = success ?  MSG_SAVE_SUCCESS: ERR_FILE_SAVE_FAILED;
    ui->labelStatus->setText(message);
    ui->progressBar->setValue(ui->progressBar->maximum());
}

void SaveProgressDialog::saveOutput(OutputType ftype, ImageList images, const Attributes &attributes)
{

    try
    {
        FrameFlowWriter writer(this);
        outputWriter = writer.createOutputFile(ftype);
        outputWriter->addImages(images);
        if(outputWriter != nullptr)
        {
            outputWriter->setAttrib(attributes);
            QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished, this, &SaveProgressDialog::onSaveFinishedThread);
            future = QtConcurrent::run([this]() {
                connect(outputWriter.get(), &IOutputFile::progressChanged,
                        this, &SaveProgressDialog::onProgressChanged,
                        Qt::QueuedConnection);

                connect(outputWriter.get(), &IOutputFile::saveStarted,
                        this, &SaveProgressDialog::onSaveStarted,
                        Qt::QueuedConnection);

                connect(outputWriter.get(), &IOutputFile::saveFinished,
                        this, &SaveProgressDialog::onSaveFinished,
                        Qt::QueuedConnection);

                outputWriter->save();
            });
            watcher->setFuture(future);
        }

    }
    catch (const FrameFlowException &ex)
    {
        QMessageBox::critical(this, EXPORT_FAILED, ex.message());
    }
    catch (...)
    {
        QMessageBox msg;
        msg.setIconPixmap(QPixmap(":/Icons/Resources/Icons/FrameFlow_100px.png"));
        msg.setText(ERR_UNKNOWN_SAVE);
        msg.setWindowTitle(EXPORT_FAILED);
        msg.exec();
    }
}

void SaveProgressDialog::onSaveFinishedThread()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setVisible(true);
}

void SaveProgressDialog::onAbortButtonPressed()
{
    ui->labelStatus->setText(EXPORT_CANCEL_MSG);
    ui->labelStatus->update();
    future.cancel();
    future.waitForFinished();
    if (QFile::exists(saveFilePath))
    {
        QFile::remove(saveFilePath);
    }
    this->reject();
}
