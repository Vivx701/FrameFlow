#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <Types.h>
#include <QThread>
#include "outputfilefactory.h"
#include <imagemodel.h>
#include <QPushButton>

namespace Ui {
class ProgressDialog;
}

class RenderingThread : public QThread
{
    Q_OBJECT
public:
    RenderingThread(ImageList& images, Attributes& attrib,  OutputType type, QObject* parent = nullptr)
        : QThread(parent), m_images(images), m_attrib(attrib), m_type(type)
    {
        this->setTerminationEnabled(true);
    }

protected:
    void run() override {
        std::unique_ptr<IOutputFile> output = OutputFileFactory::createOutputFile(m_type);
        connect(output.get(), &IOutputFile::progressChanged, this, [this](int max, int value) {
            emit progressChanged(max, value);
        });
        output->addImages(m_images);
        output->setAttrib(m_attrib);
        output->save();
    }

signals:
    void progressChanged(int maximum, int value);
private:
    ImageList &m_images;
    Attributes &m_attrib;
    OutputType m_type;
};

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(ImageList& images, QWidget *parent = nullptr);
    void setFilename(QString path);
    void setAttributes(Attributes &attrib, OutputType type);
    ~ProgressDialog();
    ImageList &images() const;
    int exec() override;

private slots:
    void onProgressChanged(int max, int value);
private:
    Ui::ProgressDialog *ui;
    ImageList &m_images;
    RenderingThread *rThread=nullptr;
    //Strings
    QString SAVING_MESSAGE = QObject::tr("Saving");
};

#endif // PROGRESSDIALOG_H
