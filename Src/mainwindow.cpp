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

        //saveFile("Output.png", OutputType::SPRITE);
        saveFile("Output.pdf", OutputType::PDF);
        //saveFile("genOutput.gif", OutputType::GIF);
        //saveFile("genOutput.mp4", OutputType::VIDEO);

    });

    // Add the button to the toolbar
    ui->toolBar->addWidget(fileBrowseButton);
}

void MainWindow::saveFile(QString filePath, OutputType type)
{
    std::unique_ptr<IOutputFile> output = OutputFileFactory::createOutputFile(type);
    ImageList images = m_model.getImageList();
    output->addImages(images);
    PDFAttributes pattr;
    pattr.filePath = filePath;
    pattr.specificSettings["Title"] = "SampleTitle";
    pattr.specificSettings["Fill"] = "Fit";
    pattr.specificSettings["Orientation"] = QPageLayout::Landscape;
    pattr.background = QColor::fromRgb(133, 193, 233);
    output->setAttrib(pattr);
    output->save();
    //    ImageSpriteAttributes attr;
    //    attr.filePath = filePath;
    //    attr.specificSettings["Orientation"] = Qt::Horizontal;
    //    attr.specificSettings["Format"] = "PNG";
    //    attr.specificSettings["Author"] = "Vivek P";


//    GifAttributes attr;
//    attr.filePath = filePath;
//    attr.specificSettings["Author"] = "Vivek P";
//    attr.specificSettings["FPS"] = 30;
//    attr.specificSettings["Loops"] = 0;

//    VideoAttributes attr;
//    attr.filePath = filePath;
//    attr.specificSettings["Author"] = "Vivek P";
//    attr.specificSettings["FPS"] = 30;
//    attr.specificSettings["FrameDelay"] = 5000;
//    attr.specificSettings["Copyright"] = "This is vivek";


//    output->setAttrib(attr);
//    output->save();
}

