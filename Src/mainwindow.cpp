#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Strings.h"
#include <QToolButton>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <imagedelegate.h>
#include <exportdialog.h>
#include <QClipboard>
#include <QMimeData>
#include <settingsdialog.h>
#include <progressdialog.h>
#include <QPair>
#include <QProcess>

/**
 * @brief Constructor for MainWindow
 * @param parent The parent widget
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), tempDir(new QTemporaryDir())
{
    ui->setupUi(this);
    setupUI();
}

/**
 * @brief Destructor for MainWindow
 */
MainWindow::~MainWindow()
{
    delete tempDir;
    delete ui;
}

/**
 * @brief Sets up the user interface
 */
void MainWindow::setupUI()
{
    // Create and set up the New button
    QToolButton *newButton = new QToolButton(this);
    newButton->setText(Strings::NEW_BUTTON_TEXT);
    newButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/new-box.png"));
    newButton->setToolTip(Strings::NEW_BUTTON_TOOLTIP);
    newButton->setFixedSize(QSize(75, 50));

    // Create and set up the Browse button
    QToolButton *fileBrowseButton = new QToolButton(this);
    fileBrowseButton->setText(Strings::BROWSE_BUTTON_TEXT);
    fileBrowseButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/plus-box.png"));
    fileBrowseButton->setToolTip(Strings::BROWSE_BUTTON_TOOLTIP);
    fileBrowseButton->setFixedSize(QSize(75, 50));

    // Create and set up the Browse button
    QToolButton *addFromClipboardButton = new QToolButton(this);
    addFromClipboardButton->setText(Strings::CBOARD_BUTTON_TEXT);
    addFromClipboardButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/paste.png"));
    addFromClipboardButton->setToolTip(Strings::CLIPBRD_BUTTON_TOOLTIP);
    addFromClipboardButton->setFixedSize(QSize(75, 50));

    // Create and set up the Export button
    QToolButton *exportButton = new QToolButton(this);
    exportButton->setText(Strings::EXPORT_BUTTON_TEXT);
    exportButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/export.png"));
    exportButton->setToolTip(Strings::EXPORT_BUTTON_TOOLTIP);
    exportButton->setFixedSize(QSize(75, 50));

    // Create and set up the Settings button
    QToolButton *settingsButton = new QToolButton(this);
    settingsButton->setText(Strings::EXPORT_BUTTON_TEXT);
    settingsButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/cog.png"));
    settingsButton->setToolTip(Strings::SETTINGS_BUTTON_TOOLTIP);
    settingsButton->setFixedSize(QSize(75, 50));

    // Create and set up the About button
    QToolButton *aboutButton = new QToolButton(this);
    aboutButton->setText(Strings::EXPORT_BUTTON_TEXT);
    aboutButton->setIcon(QIcon(":/Dark/icons/Resources/Icons/dark/information-outline.png"));
    aboutButton->setToolTip(Strings::ABOUT_BUTTON_TOOLTIP);
    aboutButton->setFixedSize(QSize(75, 50));

    // Add the buttons to the toolbar
    ui->SideBar->setIconSize(QSize(50, 50));
    QWidget *spacer = new QWidget(this);
    spacer->setFixedHeight(200);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    ui->SideBar->addWidget(spacer);
    ui->SideBar->addWidget(newButton);
    ui->SideBar->addWidget(fileBrowseButton);
    ui->SideBar->addWidget(addFromClipboardButton);
    ui->SideBar->addWidget(exportButton);
    ui->SideBar->addSeparator();
    ui->SideBar->addWidget(settingsButton);
    ui->SideBar->addWidget(aboutButton);

    // Set up the list view and graphics view
    ui->imageListView->setModel(&m_model);
    ImageDelegate *delegate = new ImageDelegate(this);
    ui->imageListView->setItemDelegate(delegate);
    ui->graphicsView->setScene(&m_scene);
    ui->graphicsView->setAlignment(Qt::AlignCenter);
    m_scene.addItem(&m_pixmapItem);

    // Connect the list view clicked signal
    QObject::connect(ui->imageListView, &QListView::clicked, [this](const QModelIndex& index) {
        QImage img(index.data(Qt::DisplayRole).toString());
        if(!img.isNull())
        {
            m_pixmapItem.setPixmap(QPixmap(index.data(Qt::DisplayRole).toString()));
            m_scene.setSceneRect(m_pixmapItem.boundingRect());
            ui->graphicsView->fitInView(m_pixmapItem.boundingRect(), Qt::KeepAspectRatio);
            ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        }
        QVector<QPair<QString, QString>> imageProperties = m_model.getSelectedImageProperties(index);

        // Add new contents
        clearProperties();
        for (const auto& property : imageProperties) {
            QLineEdit *secondText = new QLineEdit(property.second, this);
            secondText->setFont(qApp->font());
            secondText->setReadOnly(true);
            ui->propertiesLayout->addRow(property.first, secondText);

            if(property.first == Strings::MODIFIED_PROPERTY)
            {
                ui->propertiesLayout->addItem(new QSpacerItem(50, 30, QSizePolicy::Expanding, QSizePolicy::Fixed));
            }
        }
    });

    // Connect the New button clicked signal
    connect(newButton, &QToolButton::clicked, this, [this](){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, Strings::CONFIRM_NEW_TITLE,
                                      Strings::CONFIRM_NEW_MESSAGE,
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            resetAll();
        }
    });

    // Connect the Browse button clicked signal
    connect(fileBrowseButton, &QToolButton::clicked, this, [this](){
        QStringList fileNames = QFileDialog::getOpenFileNames(this, Strings::OPEN_IMAGE_DIALOG_TITLE, QDir::homePath(),
                                                              Strings::IMAGE_FILES_FILTER);

        for(QString name: fileNames)
        {
            m_model.addImage(name);
        }
    });

    // Connect the Export button clicked signal
    connect(exportButton, &QToolButton::clicked, this, [this](){

        if(m_model.rowCount() < 1)
        {
            emit showErrorMessage(Strings::NO_IMAGES_TITLE, Strings::NO_IMAGES_MSG, QMessageBox::Information);
            return;
        }

        ExportDialog exportDialog;
        exportDialog.loadTheme(currentTheme());
        if(QDialog::Accepted == exportDialog.exec())
        {
            QMap<QString, OutputType> outputMap = {
                {"pdf", OutputType::PDF},
                {"video", OutputType::VIDEO},
                {"sprite", OutputType::SPRITE},
                {"gif", OutputType::GIF},
                {"html", OutputType::HTMLGALLERY},
            };
            Attributes attrib = exportDialog.exportSettings();
            writeFile(attrib, outputMap[exportDialog.currentTabName()]);
        }
    });

    // Connect the addFromClipboardButton  clicked signal
    connect(addFromClipboardButton, &QToolButton::clicked, this, [this](){

        QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();

        if (mimeData->hasImage()) {
            QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull()) {
                QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
                QString fileName =  QString("clipboard_image_%1.png").arg(timestamp);
                QString filePath = tempDir->filePath(fileName);

                if (image.save(filePath, "PNG")) {
                    qDebug() << "Image saved to:" << filePath;
                     m_model.addImage(filePath);

                } else {
                    emit showErrorMessage(Strings::FAILED_CBOARD_TITLE, Strings::FAILED_CBOARD_MSG, QMessageBox::Critical);
                }
            }
        }
        else
        {
            emit showErrorMessage(Strings::NO_CBOARD_TITLE, Strings::NO_CBOARD_MSG, QMessageBox::Information);
        }

    });

    // Connect the Settings  clicked signal
    connect(settingsButton, &QToolButton::clicked, this, [this](){
        settingsDialog sDialog;
        sDialog.setFixedSize(600, 250);
        if(QDialog::Accepted == sDialog.exec())
        {
            QSettings settings;
            QMap<QString, QString> values =  sDialog.Settings();
            for (QMap<QString, QString>::const_iterator it = values.constBegin(); it != values.constEnd(); ++it) {
                settings.setValue(it.key(), it.value());
            }
            applySettings();
        }
    });

    //Show error message.
    connect(this, &MainWindow::showErrorMessage, this, [](QString title, QString msg, QMessageBox::Icon icon){
        QMessageBox msgBox;
        msgBox.setIcon(icon);
        msgBox.setText(title);
        msgBox.setInformativeText(msg);
        msgBox.setWindowTitle(SOFTWARENAME);
        msgBox.setFixedSize(600, 300);
        msgBox.exec();
    });
}

/**
 * @brief Writes the output file
 * @param attrib The attributes for the output
 * @param type The type of output file
 */
void MainWindow::writeFile(Attributes &attrib, OutputType type)
{

    ImageList images = m_model.getImageList();
    ProgressDialog pDialog(images, this);
    pDialog.setFilename(attrib.filePath);
    pDialog.setFixedSize(640, 200);
    pDialog.setAttributes(attrib, type);
    pDialog.exec();
}

/**
 * @brief Resets all data and UI elements
 */
void MainWindow::resetAll()
{
    m_model.clearImages();
    ui->imageListView->reset();
    clearProperties();
    m_pixmapItem.setPixmap(QPixmap());
}

/**
 * @brief Clears the properties layout
 */
void MainWindow::clearProperties()
{
    while (ui->propertiesLayout->rowCount() > 0)
    {
        ui->propertiesLayout->removeRow(0);
    }
}

void MainWindow::applySettings()
{
    QSettings settings;
    // Read a value
    QString lang = settings.value(LANGUAGE, "English").toString();
    QString icontheme = settings.value(ICONTHEME, "Default").toString();
    QString colortheme = settings.value(COLORTHEME, "Default").toString();
    QString fontfamily = settings.value(FONT, "Monospace").toString();

    qApp->setProperty(LANGUAGE.toLocal8Bit().data(), lang);
    qApp->setProperty(ICONTHEME.toLocal8Bit().data(), icontheme);
    qApp->setProperty(COLORTHEME.toLocal8Bit().data(), colortheme);
    qApp->setProperty(FONT.toLocal8Bit().data(), fontfamily);

    // Set App font
    QFont globalFont(fontfamily);  // Font family and size
    qApp->setFont(globalFont);

    QFile file(ThemeMap[colortheme]);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

QString MainWindow::currentTheme()
{
    QString themeName = qApp->property(COLORTHEME.toLocal8Bit().data()).toString();
    return ThemeMap[themeName];
}



/**
 * @brief Handles the left button click
 */
void MainWindow::on_leftButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->imageListView->currentIndex();
    int newRow = (currentIndex.row() == 0)? (m_model.rowCount()-1): (currentIndex.row()-1);
    ui->imageListView->setCurrentIndex(m_model.index(newRow, 0));
}

/**
 * @brief Handles the right button click
 */
void MainWindow::on_rightButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->imageListView->currentIndex();
    int newRow = (currentIndex.row()+1)%m_model.rowCount();
    ui->imageListView->setCurrentIndex(m_model.index(newRow, 0));
}

/**
 * @brief Handles the delete button click
 */
void MainWindow::on_deleteButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->imageListView->currentIndex();
    m_model.removeImage(currentIndex.row());
}

/**
 * @brief Handles the move back button click
 */
void MainWindow::on_moveBackButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->imageListView->currentIndex();
    int newRow = (currentIndex.row() == 0)? (currentIndex.row()): (currentIndex.row()-1);
    m_model.moveImage(currentIndex.row(), newRow);
    ui->imageListView->setCurrentIndex(m_model.index(newRow, 0));
}

/**
 * @brief Handles the move front button click
 */
void MainWindow::on_moveFrontButton_clicked()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->imageListView->currentIndex();
    int newRow = (currentIndex.row() ==  m_model.rowCount()-1)? (currentIndex.row()): (currentIndex.row()+1);
    m_model.moveImage(currentIndex.row(), newRow);
}
