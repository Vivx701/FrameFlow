#ifndef FRAMEFLOW_H
#define FRAMEFLOW_H

#include <QMainWindow>

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

private:
    Ui::FrameFlow *ui;
};
#endif // FRAMEFLOW_H
