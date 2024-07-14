#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include<Types.h>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    QString currentTabName();
    Attributes exportSettings();
    ~ExportDialog();

private slots:
    void openFolderDialog();

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
