#ifndef SAVEPROGRESSDIALOG_H
#define SAVEPROGRESSDIALOG_H

#include <QDialog>
#include <Types.h>
#include <frameflowwriter.h>
#include <QFuture>

namespace Ui {
class SaveProgressDialog;
}

class SaveProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveProgressDialog(QWidget *parent = nullptr);
    ~SaveProgressDialog();

public slots:
    void onProgressChanged(int total, int value);
    void onSaveStarted(const QString &fileName);
    void onSaveFinished(bool success, const QString &fileName);
    void saveOutput(OutputType ftype, ImageList images, const Attributes &attributes);
    void onSaveFinishedThread();
    void onAbortButtonPressed();

private:
    Ui::SaveProgressDialog *ui;
    std::unique_ptr<IOutputFile> outputWriter;
    QFuture<void> future;
    QString saveFilePath;
};

#endif // SAVEPROGRESSDIALOG_H
