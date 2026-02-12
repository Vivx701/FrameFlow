#include "languagedialog.h"
#include "ui_languagedialog.h"

LanguageDialog::LanguageDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LanguageDialog)
{
    ui->setupUi(this);
    ui->langListWidget->addItem("English");
    ui->langListWidget->setCurrentRow(0);
}

LanguageDialog::~LanguageDialog()
{
    delete ui;
}
