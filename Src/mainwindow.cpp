#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <imagedelegate.h>
#include <exportdialog.h>

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

    QToolButton *newButton = new QToolButton(this);
    newButton->setText("New");
    newButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/new-box.png"));
    newButton->setToolTip("Start a new project");
    newButton->setFixedSize(QSize(75, 50));

    QToolButton *fileBrowseButton = new QToolButton(this);
    fileBrowseButton->setText("Browse");
    fileBrowseButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/plus-box.png"));
    fileBrowseButton->setToolTip("Add images to project");
    fileBrowseButton->setFixedSize(QSize(75, 50));

    QToolButton *exportButton = new QToolButton(this);
    exportButton->setText("Export");
    exportButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/export.png"));
    exportButton->setToolTip("Export the images ");
    exportButton->setFixedSize(QSize(75, 50));

    QToolButton *settingsButton = new QToolButton(this);
    settingsButton->setText("Export");
    settingsButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/cog.png"));
    settingsButton->setToolTip("Settings");
    settingsButton->setFixedSize(QSize(75, 50));

    QToolButton *aboutButton = new QToolButton(this);
    aboutButton->setText("Export");
    aboutButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/information-outline.png"));
    aboutButton->setToolTip("About screen");
    aboutButton->setFixedSize(QSize(75, 50));


    // Add the buttons to the toolbar
    ui->toolBar->setIconSize(QSize(50, 50));
    QWidget *spacer = new QWidget(this);
    spacer->setFixedHeight(100);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addWidget(newButton);
    ui->toolBar->addWidget(fileBrowseButton);
    ui->toolBar->addWidget(exportButton);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(settingsButton);
    ui->toolBar->addWidget(aboutButton);



    ui->listView->setModel(&m_model);
    ImageDelegate *delegate = new ImageDelegate(this);
    ui->listView->setItemDelegate(delegate);
    ui->graphicsView->setScene(&m_scene);
    ui->graphicsView->setAlignment(Qt::AlignCenter);
    m_scene.addItem(&m_pixmapItem);

    QObject::connect(ui->listView, &QListView::clicked,  [this](const QModelIndex& index) {

        QImage img(index.data(Qt::DisplayRole).toString());
        if(!img.isNull())
        {
            m_pixmapItem.setPixmap(QPixmap(index.data(Qt::DisplayRole).toString()));
            m_scene.setSceneRect(m_pixmapItem.boundingRect());
            ui->graphicsView->fitInView(m_pixmapItem.boundingRect(), Qt::KeepAspectRatio);
            ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        }
        QVector<QPair<QString, QString>> imageProperties = m_model.getSelectedImageProperties(index);



        //Add new contents
        clearProperties();
        for (const auto& property : imageProperties) {
            QLineEdit *secondText = new QLineEdit(property.second, this);
            secondText->setReadOnly(true);
            ui->propertiesLayout->addRow(property.first, secondText);
            if(property.first == "Modified")
            {
                ui->propertiesLayout->addItem(new QSpacerItem(50, 30, QSizePolicy::Expanding, QSizePolicy::Fixed));
            }
        }


    });


    // Connect the clicked signal to a slot
    connect(newButton, &QToolButton::clicked, this, [this](){

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm New",
                                      "Are you sure you want to start again ?\nAll changes will be lost.",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // User confirmed, proceed with clearing
            resetAll();
        }

    });

    connect(fileBrowseButton, &QToolButton::clicked, this, [this](){

        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Image"), QDir::homePath(),
                                                              tr("Image Files (*.png *.jpg *.bmp)"));

        for(QString name: fileNames)
        {
            m_model.addImage(name);
        }

        // saveFile("Output.png", OutputType::SPRITE);
        //saveFile("Output.pdf", OutputType::PDF);
        //saveFile("genOutput.gif", OutputType::GIF);
        //saveFile("genOutput.mp4", OutputType::VIDEO);

        saveFile("genOutputHTML", OutputType::HTMLGALLERY);


    });

    connect(exportButton, &QToolButton::clicked, this, [this](){

        ExportDialog exportDialog;
        if(QDialog::Accepted == exportDialog.exec())
        {

        }


    });

}

void MainWindow::saveFile(QString filePath, OutputType type)
{
    std::unique_ptr<IOutputFile> output = OutputFileFactory::createOutputFile(type);
    ImageList images = m_model.getImageList();
    output->addImages(images);
    //    PDFAttributes pattr;
    //    pattr.filePath = filePath;
    //    pattr.specificSettings["Title"] = "SampleTitle";
    //    pattr.specificSettings["Fill"] = "Fit";
    //    pattr.specificSettings["Orientation"] = QPageLayout::Landscape;
    //    pattr.background = QColor::fromRgb(133, 193, 233);
    //    output->setAttrib(pattr);
    //    output->save();
    //        ImageSpriteAttributes attr;
    //        attr.filePath = filePath;
    //        attr.specificSettings["Orientation"] = Qt::Horizontal;
    //        attr.specificSettings["Format"] = "PNG";
    //        attr.specificSettings["Author"] = "Vivek P";


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

    //    ImageCollageAttributes attr;
    //    attr.filePath = filePath;
    //    attr.specificSettings["Format"] = "PNG";
    //    attr.specificSettings["Author"] = "Vivek P";


    HTMLGalleryAttributes attr;
    attr.filePath = filePath;
    attr.specificSettings["Author"] = "Vivek P";
    attr.specificSettings["Copyright"] = "This is vivek";
    attr.specificSettings["EnableLightbox"] = false;
    output->setAttrib(attr);
    output->save();
}

void MainWindow::resetAll()
{
    m_model.clearImages();
    ui->listView->reset();
    clearProperties();
    m_pixmapItem.setPixmap(QPixmap());
}

void MainWindow::clearProperties()
{
    //clear the existing contents
    while ( ui->propertiesLayout->rowCount() > 0)
    {
        ui->propertiesLayout->removeRow(0);
    }
}


void MainWindow::on_leftButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->listView->currentIndex();
    int newRow = (currentIndex.row() == 0)? (m_model.rowCount()-1): (currentIndex.row()-1);
    ui->listView->setCurrentIndex(m_model.index(newRow, 0));
}


void MainWindow::on_rightButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->listView->currentIndex();
    int newRow = (currentIndex.row()+1)%m_model.rowCount();
    ui->listView->setCurrentIndex(m_model.index(newRow, 0));
}


void MainWindow::on_deleteButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->listView->currentIndex();
    m_model.removeImage(currentIndex.row());
}



void MainWindow::on_moveBackButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->listView->currentIndex();
    int newRow = (currentIndex.row() == 0)? (currentIndex.row()): (currentIndex.row()-1);
    m_model.moveImage(currentIndex.row(), newRow);
    ui->listView->setCurrentIndex(m_model.index(newRow, 0));
}


void MainWindow::on_moveFrontButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->listView->currentIndex();
    int newRow = (currentIndex.row() ==  m_model.rowCount()-1)? (currentIndex.row()): (currentIndex.row()+1);
    m_model.moveImage(currentIndex.row(), newRow);
}

