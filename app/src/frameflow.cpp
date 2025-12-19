#include "frameflow.h"
#include "ui_frameflow.h"
#include "languagedialog.h"
#include "appearancedialog.h"
#include "videoattributesdialog.h"
#include "gifattributesdialog.h"
#include "imagespriteattributesdialog.h"
#include "saveprogressdialog.h"

#include <frameflowstrings.h>
#include <QClipboard>
#include <QMimeData>
#include <QFileDialog>
#include <QDir>
#include <QCborMap>
#include <QCborArray>
#include <QCborValue>
#include <QCborStreamWriter>
#include <QSettings>
#include <QTimer>

FrameFlow::FrameFlow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FrameFlow)
{
    ui->setupUi(this);
    setupUI();
    applySettings();
}

FrameFlow::~FrameFlow()
{
    delete ui;
}

bool FrameFlow::loadProjectFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit showErrorMessage(LOAD_PROJECT_TITLE, LOAD_OPEN_FAILED_MSG, QMessageBox::Critical);
        return false;
    }
    QCborValue value = QCborValue::fromCbor(file.readAll());
    if (!value.isMap())
    {
        emit showErrorMessage(LOAD_PROJECT_TITLE, LOAD_PARSE_FAILED_MSG, QMessageBox::Critical);
        return false;
    }

    QCborMap root = value.toMap();

    int version = root.value(QString(KEY_VERSION)).toInteger(1);
    if (version != PROJECTFILEVERSION)
    {
        emit showErrorMessage(LOAD_PROJECT_TITLE, LOAD_VERSION_UNSUPPORTED_MSG, QMessageBox::Critical);
        return false;
    }
    // 🔹 Read save time
    QDateTime lastSaved = root.value(QString(KEY_LASTSAVED)).toDateTime();
    // images
    QCborValue values =  root.value(QString(KEY_IMAGES));
    if(!values.isArray())
    {
        emit showErrorMessage(LOAD_PROJECT_TITLE, LOAD_PARSE_FAILED_MSG, QMessageBox::Critical);
        return false;
    }
    resetAll();
    bool imagesLoaded = true;
    for(QVariant value: values.toArray().toVariantList())
    {
        QString path = value.toString();
        if(QFile(path).exists())
        {
            m_model.addImage(path);
        }
        else
        {
            imagesLoaded = false;
        }
    }

    if(!imagesLoaded)
    {
        emit showErrorMessage(LOAD_PROJECT_TITLE, LOAD_IMAGES_MISSING_MSG, QMessageBox::Warning);
    }
    QCborValue currentIndex =  root.value(QString(KEY_INDEX));
    if(currentIndex.isInteger())
    {
        QModelIndex index = m_model.index(currentIndex.toInteger(), 0);
        if(index.isValid())
        {
            ui->imageListView->setCurrentIndex(index);
        }
    }

    m_themeManager.reload();
    return true;
}

bool FrameFlow::writeProjectFile(const QString &filePath)
{
    QCborMap root;
    root.insert(QString(KEY_VERSION), 1);
    root.insert(QString(KEY_LASTSAVED),  QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
    root.insert(QString(KEY_INDEX),  ui->imageListView->currentIndex().row());

    QCborArray images;
    for (int row = 0; row < m_model.rowCount(); ++row)
    {
        QModelIndex index = m_model.index(row, 0);
        images.append(m_model.data(index, Qt::DisplayRole).toString());
    }

    root.insert(QString(KEY_IMAGES), images);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    file.write(QCborValue(root).toCbor());
    return true;
}

void FrameFlow::applySettings()
{

    QSettings settings;
    QString iconTheme = settings.value(SETTINGS_ICON, DEFAULTTHEMEICON).toString();
    QString styleTheme = settings.value(SETTINGS_STYLE, DEFAULTSTYLE).toString();

    if(m_themeManager.currentStyle() != styleTheme)
    {
        m_themeManager.setStyleTheme(styleTheme);
    }

    if(m_themeManager.currentIconTheme() != iconTheme)
    {
        m_themeManager.setIconTheme(iconTheme);
    }
}

void FrameFlow::setupUI()
{
    //Set icons
    ui->leftButton->setIcon(m_themeManager.getIcon(ICONTYPE::LEFT_BUTTON));
    ui->rightButton->setIcon(m_themeManager.getIcon(ICONTYPE::RIGHT_BUTTON));
    ui->moveBackButton->setIcon(m_themeManager.getIcon(ICONTYPE::MOVEBACK_BUTTON));
    ui->moveFrontButton->setIcon(m_themeManager.getIcon(ICONTYPE::MOVEFRONT_BUTTON));
    ui->deleteButton->setIcon(m_themeManager.getIcon(ICONTYPE::DELETE_BUTTON));

    // Set up the list view and graphics view
    ui->imageListView->setModel(&m_model);
    ImageDelegate *delegate = new ImageDelegate(this);
    ui->imageListView->setItemDelegate(delegate);
    ui->graphicsView->setScene(&m_scene);
    ui->graphicsView->setAlignment(Qt::AlignCenter);
    m_scene.addItem(&m_pixmapItem);
    ui->imageName->setText(NO_IMAGES_SELECTED);
    // Anchor view scaling to center
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    // Smooth scaling when resizing
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    // Efficient viewport updates
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    //icon theme change
    connect(&m_themeManager, &ThemeManager::iconThemeChanged, this, [this](){

        //Set icons
        ui->action_New->setIcon(m_themeManager.getIcon(ICONTYPE::NEW_BUTTON));
        ui->action_Open->setIcon(m_themeManager.getIcon(ICONTYPE::BROWSE_BUTTON));
        ui->actionPaste_Image->setIcon(m_themeManager.getIcon(ICONTYPE::CBOARD_BUTTON));
        ui->actionAbout->setIcon(m_themeManager.getIcon(ICONTYPE::ABOUT_BUTTON));
        ui->action_Language->setIcon(m_themeManager.getIcon(ICONTYPE::SETTINGS_BUTTON));

        ui->leftButton->setIcon(m_themeManager.getIcon(ICONTYPE::LEFT_BUTTON));
        ui->rightButton->setIcon(m_themeManager.getIcon(ICONTYPE::RIGHT_BUTTON));
        ui->moveBackButton->setIcon(m_themeManager.getIcon(ICONTYPE::MOVEBACK_BUTTON));
        ui->moveFrontButton->setIcon(m_themeManager.getIcon(ICONTYPE::MOVEFRONT_BUTTON));
        ui->deleteButton->setIcon(m_themeManager.getIcon(ICONTYPE::DELETE_BUTTON));
        statusBar()->showMessage(THEME_APPLY_SUCCESS_MSG, 5000);
        QSettings settings;
        settings.setValue(SETTINGS_ICON, m_themeManager.currentIconTheme());
    });

    connect(&m_themeManager, &ThemeManager::styleThemeChanged, this, [this](){

        QFile file(m_themeManager.getCurrentStyleSheet());
        if(file.exists())
        {
            file.open(QFile::ReadOnly);
            QString styleSheet = QLatin1String(file.readAll());
            qApp->setStyleSheet(styleSheet);
            statusBar()->showMessage(THEME_APPLY_SUCCESS_MSG, 5000);
            QSettings settings;
            settings.setValue(SETTINGS_STYLE, m_themeManager.currentStyle());
        }
        else
        {
            emit showErrorMessage(THEME_APPLY_FAILED_MSG, THEME_APPLY_FAILED_MSG, QMessageBox::Warning);
        }
    });

    //Show error message.
    connect(this, &FrameFlow::showErrorMessage, this, [](QString title, QString msg, QMessageBox::Icon icon){
        QMessageBox msgBox;
        msgBox.setIcon(icon);
        msgBox.setText(title);
        msgBox.setInformativeText(msg);
        msgBox.setWindowTitle(SOFTWARENAME);
        msgBox.setFixedSize(600, 300);
        msgBox.exec();
    });

    //Menu Handling --------------------
    // Connect the addFromClipboardButton  clicked signal
    connect(ui->actionPaste_Image, &QAction::triggered, this, [this](){

        QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();

        if (mimeData->hasImage()) {
            QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull()) {
                QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
                QString fileName =  QString("clipboard_image_%1.png").arg(timestamp);
                QString filePath = m_tempDir.filePath(fileName);

                if (image.save(filePath, "PNG")) {
                    qDebug() << "Image saved to:" << filePath;
                    m_model.addImage(filePath);

                } else {
                   emit showErrorMessage(FAILED_CBOARD_TITLE, FAILED_CBOARD_MSG, QMessageBox::Critical);
                }
            }
        }
        else
        {
            emit showErrorMessage(NO_CBOARD_TITLE, NO_CBOARD_MSG, QMessageBox::Information);
        }

    });

    // Connect the New button clicked signal
    connect(ui->action_New, &QAction::triggered, this, [this](){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, CONFIRM_NEW_TITLE,
                                      CONFIRM_NEW_MESSAGE,
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            resetAll();
        }
    });

    // Connect the add images clicked signal
    connect(ui->actionAdd_Image, &QAction::triggered, this, [this](){
        QStringList fileNames = QFileDialog::getOpenFileNames(this, OPEN_IMAGE_DIALOG_TITLE, QDir::homePath(),
                                                              IMAGE_FILES_FILTER);

        for(QString name: fileNames)
        {
            m_model.addImage(name);
        }
    });

    // Connect the remove images clicked signal
    connect(ui->actionRemove, &QAction::triggered, this, [this](){
        removeSelectedImage();
    });

    // Connect the move back  clicked signal
    connect(ui->actionMove_back, &QAction::triggered, this, [this](){
        moveSelectedImageBack();
    });

    // Connect the move front clicked signal
    connect(ui->actionMove_front, &QAction::triggered, this, [this](){
        moveSelectedImageFront();
    });


    connect(ui->actionAbout, &QAction::triggered, [](){AboutDialog about;about.exec();});
    connect(ui->action_Language, &QAction::triggered, [](){LanguageDialog lang;lang.exec();});
    connect(ui->action_Appearance, &QAction::triggered, [this](){AppearanceDialog app(m_themeManager);app.exec();});
    connect(ui->actionS_ave_As, &QAction::triggered, this, &FrameFlow::saveProjectAs);
    connect(ui->action_Open, &QAction::triggered, this, &FrameFlow::openProject);
    connect(ui->action_Video, &QAction::triggered, this, [this](){

        if(m_model.getImageList().isEmpty())
        {
            QMessageBox::information(this, NO_IMAGES_TITLE, NO_IMAGES_MSG);
            return;
        }
        VideoAttributes attrs;
        VideoAttributesDialog dlg(attrs);
        if (dlg.exec() == QDialog::Accepted)
        {
            SaveProgressDialog sDialog;
            sDialog.saveOutput(OutputType::VIDEO, m_model.getImageList(), attrs);
            sDialog.exec();
        }
    });
    connect(ui->action_Gif, &QAction::triggered, this, [this](){
        if(m_model.getImageList().isEmpty())
        {
            QMessageBox::information(this, NO_IMAGES_TITLE, NO_IMAGES_MSG);
            return;
        }
        GifAttributes attrs; // default Delay = 100
        GifAttributesDialog dlg(attrs);
        if (dlg.exec() == QDialog::Accepted) {
            SaveProgressDialog sDialog;
            sDialog.saveOutput(OutputType::GIF, m_model.getImageList(), attrs);
            sDialog.exec();
        }

    });

    connect(ui->action_Image_sprite, &QAction::triggered, this, [this](){
        if(m_model.getImageList().isEmpty())
        {
            QMessageBox::information(this, NO_IMAGES_TITLE, NO_IMAGES_MSG);
            return;
        }
        ImageSpriteAttributes attrs;  // defaults applied in constructor
        ImageSpriteAttributesDialog dlg(attrs);
        if (dlg.exec() == QDialog::Accepted)
        {
            SaveProgressDialog sDialog;
            sDialog.saveOutput(OutputType::SPRITE, m_model.getImageList(), attrs);
            sDialog.exec();
        }

    });

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
        setImageDetails(m_model.getSelectedImageProperties(index));
    });
}

void FrameFlow::resetAll()
{
    m_model.clearImages();
    ui->imageListView->reset();
    m_pixmapItem.setPixmap(QPixmap());
    clearProperties();
    m_currentProjectFile.clear();
    ui->imageName->setText(NO_IMAGES_SELECTED);
}

void FrameFlow::clearProperties()
{
    ui->imageName->clear();
}

void FrameFlow::setImageDetails(QVector<QPair<QString, QString>> imageProperties)
{
    QFontMetrics fm(ui->imageName->font());
    QString infoText = QString(
                           R"(
   <div>
  <table cellspacing="4" cellpadding="0">
    <tr>
      <td style="padding-right:12px;">Filename:</td>
      <td>%1</td>
    </tr>
    <tr>
      <td style="padding-right:12px;">Resolution:</td>
      <td>%2 × %3 </td>
    </tr>
    <tr>
      <td style="padding-right:12px;">Index:</td>
      <td>%4 </td>
    </tr>
  </table>
</div>
)"
                           )
                           .arg(fm.elidedText(imageProperties[0].second, Qt::ElideRight, 200))
                           .arg(imageProperties[5].second)
                           .arg(imageProperties[4].second)
                           .arg(imageProperties.last().second);

    ui->imageName->setText(infoText);
    ui->imageName->setTextFormat(Qt::RichText);

}

void FrameFlow::saveProjectAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Project As"), m_currentProjectFile.isEmpty() ? QDir::homePath(): QDir(m_currentProjectFile).absolutePath(), tr("FrameFlow Project (*.ffp)"));
    if (filePath.isEmpty())
    {
        return;
    }
    // Ensure extension
    if (!filePath.endsWith(".ffp"))
    {
        filePath += ".ffp";
    }

    if (!writeProjectFile(filePath))
    {
        emit showErrorMessage(SAVE_FAILED_TITLE, SAVE_WRITE_FAILED_MSG, QMessageBox::Critical);
        return;
    }

    m_currentProjectFile = filePath;
    statusBar()->showMessage(SAVE_SUCCESS_MSG+"  "+QFileInfo(filePath).fileName(), 5000);
}

void FrameFlow::saveProject()
{
    // If never saved before → Save As
    if (m_currentProjectFile.isEmpty())
    {
        saveProjectAs();
        return;
    }

    if (!writeProjectFile(m_currentProjectFile))
    {
        emit showErrorMessage(SAVE_FAILED_TITLE, SAVE_WRITE_FAILED_MSG, QMessageBox::Critical);
        return;
    }
    statusBar()->showMessage(SAVE_SUCCESS_MSG+"  "+QFileInfo(m_currentProjectFile).fileName(), 5000);
}

void FrameFlow::openProject()
{
    QString fileName = QFileDialog::getOpenFileName(this, LOAD_PROJECT_TITLE, m_currentProjectFile.isEmpty() ? QDir::homePath(): QDir(m_currentProjectFile).absolutePath(), PROJECT_FILES_FILTER);
    if(fileName.isEmpty())
    {
        return;
    }

    if (!QFile(fileName).exists())
    {
        emit showErrorMessage(LOAD_FAILED_TITLE, LOAD_OPEN_FAILED_MSG, QMessageBox::Critical);
        return;
    }
    if(loadProjectFile(fileName))
    {
        m_currentProjectFile = fileName;
        statusBar()->showMessage(QString(LOADING_MESSAGE)+"  "+fileName, 2000);
    }
}

void FrameFlow::removeSelectedImage()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->imageListView->currentIndex();
    m_model.removeImage(currentIndex.row());

    if(m_model.rowCount() == 0)
    {
        resetAll();
    }
}

void FrameFlow::moveSelectedImageBack()
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

void FrameFlow::moveSelectedImageFront()
{
    if(m_model.rowCount() == 0)
    {
        return;
    }
    QModelIndex currentIndex = ui->imageListView->currentIndex();
    int newRow = (currentIndex.row() ==  m_model.rowCount()-1)? (currentIndex.row()): (currentIndex.row()+1);
    m_model.moveImage(currentIndex.row(), newRow);
}

/**
 * @brief Handles the left button click
 */
void FrameFlow::on_leftButton_clicked()
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
void FrameFlow::on_rightButton_clicked()
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
 * @brief Handles the move back button click
 */
void FrameFlow::on_moveBackButton_clicked()
{
    moveSelectedImageBack();
}

/**
 * @brief Handles the move front button click
 */
void FrameFlow::on_moveFrontButton_clicked()
{
    moveSelectedImageFront();
}

/**
 * @brief Handles the delete button click
 */
void FrameFlow::on_deleteButton_clicked()
{
    removeSelectedImage();
}

/**
 * @brief Handles window state change events.
 *
 * Responds to window state transitions such as maximize, restore,
 * minimize, or fullscreen by updating the graphics view so that
 * the scene content is rendered correctly for the new window state.
 *
 * @param event The event describing the window state change.
 */

void FrameFlow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {

        // Delay until Qt finishes layout
        QTimer::singleShot(0, this, [this]() {
            updateGraphicsView();
        });
    }
    QMainWindow::changeEvent(event);
}

/**
 * @brief Handles window resize events.
 *
 * Recalculates and updates the graphics view layout to ensure
 * the displayed image or scene content is scaled and positioned
 * correctly after the window size changes.
 *
 * @param event The resize event containing the new window size.
 */

void FrameFlow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // Refit image after resize
    QTimer::singleShot(0, this, [this]() {
        updateGraphicsView();
    });
}

/** @brief Updates the graphics view. */
void FrameFlow::updateGraphicsView()
{
    if (m_pixmapItem.pixmap().isNull())
    {
        return;
    }
    // Set scene rect exactly to image size
    ui->graphicsView->setSceneRect(m_pixmapItem.boundingRect());

    // Scale image to fit view while keeping aspect ratio
    ui->graphicsView->fitInView(m_pixmapItem.boundingRect(), Qt::KeepAspectRatio);
}

