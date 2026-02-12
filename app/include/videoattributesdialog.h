#ifndef VIDEOATTRIBUTESDIALOG_H
#define VIDEOATTRIBUTESDIALOG_H

#include <QDialog>
#include <Types.h>

namespace Ui {
class VideoAttributesDialog;
}

class VideoAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoAttributesDialog(VideoAttributes &attrs, QWidget *parent = nullptr);
    ~VideoAttributesDialog();

private:
    Ui::VideoAttributesDialog *ui;
    VideoAttributes &m_attrs;

private slots:
    void accept() override;
    void onBrowseClicked();
};

#endif // VIDEOATTRIBUTESDIALOG_H
