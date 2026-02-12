#ifndef APPEARANCEDIALOG_H
#define APPEARANCEDIALOG_H

#include <QDialog>
#include <thememanager.h>
#include <Types.h>

namespace Ui {
class AppearanceDialog;
}

class AppearanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppearanceDialog(ThemeManager &themeManager, QWidget *parent = nullptr);
    ~AppearanceDialog();

private:
    Ui::AppearanceDialog *ui;
    ThemeManager &m_theme;
};

#endif // APPEARANCEDIALOG_H
