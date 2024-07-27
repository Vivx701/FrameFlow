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
    QString currentSavePath();
    void fillAttribMap();
    void fillComboBoxValues();
    Attributes exportSettings();
    void loadTheme(QString path);
    ~ExportDialog();

private slots:
    void openFolderDialog();

private:
    Ui::ExportDialog *ui;
    QMap<QString, std::function<Attributes()>> m_attributeMap;
};

#endif // EXPORTDIALOG_H
