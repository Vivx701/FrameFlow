#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <imagemodel.h>
#include "outputfilefactory.h"

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
    void saveFile(QString filePath, OutputType type);

private:
    Ui::MainWindow *ui;
    ImageModel m_model;
};
#endif // MAINWINDOW_H
