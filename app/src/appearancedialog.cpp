#include "appearancedialog.h"
#include "ui_appearancedialog.h"
#include <QSettings>

AppearanceDialog::AppearanceDialog(ThemeManager &themeManager, QWidget *parent):
    QDialog(parent),
    m_theme(themeManager),
    ui(new Ui::AppearanceDialog)
{
    ui->setupUi(this);
    QStringList iconList = m_theme.getIconThemeList();
    QStringList styleList = m_theme.getStyleThemeList();
    ui->iconListWidget->addItems(iconList);
    ui->styleListWidget->addItems(styleList);
    if(ui->iconListWidget->count() > 0)
    {
        ui->iconListWidget->setCurrentRow(0);
    }
    if(ui->styleListWidget->count() > 0)
    {
        ui->styleListWidget->setCurrentRow(0);
    }
    connect(this, &AppearanceDialog::accepted, [this](){
            m_theme.setStyleTheme(ui->styleListWidget->currentItem()->text());
            m_theme.setIconTheme(ui->iconListWidget->currentItem()->text());
    });

    QSettings settings;
    QString iconTheme = settings.value(SETTINGS_ICON, DEFAULTTHEMEICON).toString();
    QString styleTheme = settings.value(SETTINGS_STYLE, DEFAULTSTYLE).toString();

    if(int index = iconList.indexOf(iconTheme); index != -1)
    {
        ui->iconListWidget->setCurrentRow(index);
    }
    if(int index = styleList.indexOf(styleTheme); index != -1)
    {
        ui->styleListWidget->setCurrentRow(index);
    }

}

AppearanceDialog::~AppearanceDialog()
{
    delete ui;
}
