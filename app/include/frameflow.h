#ifndef FRAMEFLOW_H
#define FRAMEFLOW_H

//Qt headers
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTemporaryDir>

//local headers
#include <frameflowwriter.h>
#include <aboutDialog.h>
#include <imagemodel.h>
#include <thememanager.h>
#include <imagedelegate.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class FrameFlow;
}
QT_END_NAMESPACE

class FrameFlow : public QMainWindow
{
    Q_OBJECT

public:
    FrameFlow(QWidget *parent = nullptr);
    ~FrameFlow();
    bool loadProjectFile(const QString &filePath);
    bool writeProjectFile(const QString &filePath);
    void applySettings();

signals:
    void showErrorMessage(QString title, QString msg, QMessageBox::Icon icon);

private slots:
    void setupUI();
    void resetAll();
    void clearProperties();
    void setImageDetails(QVector<QPair<QString, QString>> imageProperties);
    void saveProjectAs();
    void saveProject();
    void openProject();

    void removeSelectedImage();
    void moveSelectedImageBack();
    void moveSelectedImageFront();

    //Handling buttons
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_moveBackButton_clicked();
    void on_moveFrontButton_clicked();
    void on_deleteButton_clicked();

protected:
    void changeEvent(QEvent *event) override;
    void updateGraphicsView();
    void resizeEvent(QResizeEvent *event);

private:
    Ui::FrameFlow *ui;
    FrameFlowWriter m_fWriter;
    ThemeManager m_themeManager;
    ImageModel m_model;
    QGraphicsScene m_scene;
    QGraphicsPixmapItem m_pixmapItem;
    QTemporaryDir m_tempDir;
    QString m_currentProjectFile;

};
#endif // FRAMEFLOW_H
