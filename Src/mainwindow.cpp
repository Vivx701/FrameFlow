#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QFileDialog>
#include <QDir>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{

    QToolButton *fileBrowseButton = new QToolButton(this);
    fileBrowseButton->setText("Click me!");
    fileBrowseButton->setIcon(QIcon(":/icons/filebrowser.png"));
    ui->listView->setModel(&m_model);



    // Connect the clicked signal to a slot
    connect(fileBrowseButton, &QToolButton::clicked, this, [this](){

        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Image"), QDir::homePath(),
                                                            tr("Image Files (*.png *.jpg *.bmp)"));

        for(QString name: fileNames)
        {
            m_model.addImage(name);
        }

        saveFile("Output.pdf", OutputType::PDF);

     });

    // Add the button to the toolbar
    ui->toolBar->addWidget(fileBrowseButton);


}

void MainWindow::saveFile(QString filePath, OutputType type)
{
    std::unique_ptr<IOutputFile> output = OutputFileFactory::createOutputFile(type);
    ImageList images = m_model.getImageList();
    output->addImages(images);
    output->save(filePath);
}

