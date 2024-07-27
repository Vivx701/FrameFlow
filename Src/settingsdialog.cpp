#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFontDatabase>
#include <Types.h>

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    fillItems();
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

void settingsDialog::fillItems()
{


    //Add Languages
    ui->languageComboBox->addItem("English", QFontDatabase::Any);
    ui->languageComboBox->addItem("Malayalam", QFontDatabase::Malayalam);
    auto langSelector = [this](){
        ui->fontList->clear();
        ui->fontList->addItems(QFontDatabase::families(ui->languageComboBox->currentData().value<QFontDatabase::WritingSystem>()));
    };
    connect(ui->languageComboBox, &QComboBox::currentTextChanged, this, langSelector);
    langSelector();
    ui->languageComboBox->setCurrentText(qApp->property(LANGUAGE.toLocal8Bit().data()).toString());
    ui->fontList->setCurrentText(qApp->property(FONT.toLocal8Bit().data()).toString());


    //ThemeList
    ui->themeList->addItems(ThemeMap.keys());
    QString themeName = qApp->property(COLORTHEME.toLocal8Bit().data()).toString();
    ui->themeList->setCurrentText(themeName);

    //icon list
    ui->iconThemeList->addItems(QStringList()<<DEFAULTTHEME<<MATERIAL);
    QString iconTheme =  qApp->property(ICONTHEME.toLocal8Bit().data()).toString();
    ui->iconThemeList->setCurrentText(iconTheme);


}

const QMap<QString, QString> settingsDialog::Settings() const
{
    QMap<QString, QString> settings;
    settings[LANGUAGE]  = ui->languageComboBox->currentText();
    settings[ICONTHEME] = ui->iconThemeList->currentText();
    settings[COLORTHEME] = ui->themeList->currentText();
    settings[FONT] = ui->fontList->currentText();
    return settings;
}
