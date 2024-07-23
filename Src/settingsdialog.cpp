#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFontDatabase>

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
}

const QMap<QString, QString> settingsDialog::Settings() const
{
    QMap<QString, QString> settings;
    settings["lang"]  = ui->languageComboBox->currentText();
    settings["icontheme"] = ui->iconThemeList->currentText();
    settings["theme"] = ui->themeList->currentText();
    settings["fontfamily"] = ui->fontList->currentText();
    settings["fontsize"] = QString::number(ui->fontSize->value());
    return settings;
}
