#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imagemodel.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMap>
#include <QLineEdit>
#include <QSpacerItem>
#include <QMessageBox>
#include <QTemporaryDir>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupUI();
    void writeFile(Attributes &attrib, OutputType type);
    void resetAll();
    void clearProperties();

private slots:
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_deleteButton_clicked();
    void on_moveBackButton_clicked();
    void on_moveFrontButton_clicked();

signals:
    void showErrorMessage(QString title, QString msg, QMessageBox::Icon icon);

private:
    Ui::MainWindow *ui;
    ImageModel m_model;
    QGraphicsScene m_scene;
    QGraphicsPixmapItem m_pixmapItem;
    QTemporaryDir *tempDir;
};
#endif // MAINWINDOW_H
