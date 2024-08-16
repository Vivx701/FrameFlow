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
    for(QLocale::Language lang: langMap.keys())
    {
        QLocale locale(lang);
        ui->languageComboBox->addItem(QLocale::languageToString(lang), locale);
    }
    auto langSelector = [this](){
        ui->fontList->clear();
        QLocale locale = ui->languageComboBox->currentData().value<QLocale>();
        ui->fontList->addItems(QFontDatabase::families(langMap[locale.language()].second));
    };
    connect(ui->languageComboBox, &QComboBox::currentTextChanged, this, langSelector);
    langSelector();

    QLocale locale(qApp->property(LANGUAGE.toLocal8Bit().data()).toString());
    ui->languageComboBox->setCurrentText(QLocale::languageToString(locale.language()));
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
    settings[LANGUAGE]  = ui->languageComboBox->currentData().value<QLocale>().name();
    settings[ICONTHEME] = ui->iconThemeList->currentText();
    settings[COLORTHEME] = ui->themeList->currentText();
    settings[FONT] = ui->fontList->currentText();
    return settings;
}
