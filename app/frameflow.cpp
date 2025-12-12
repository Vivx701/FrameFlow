#include "frameflow.h"
#include "ui_frameflow.h"

FrameFlow::FrameFlow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FrameFlow)
{
    ui->setupUi(this);
}

FrameFlow::~FrameFlow()
{
    delete ui;
}
