#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <Types.h>
#include <iconthememanager.h>

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = nullptr);
    ~settingsDialog();
    void fillItems();

    const QMap<QString, QString> Settings() const;

private:
    Ui::settingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
