#ifndef GIFATTRIBUTESDIALOG_H
#define GIFATTRIBUTESDIALOG_H

#include <QDialog>
#include <Types.h>

namespace Ui {
class GifAttributesDialog;
}

class GifAttributesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GifAttributesDialog(GifAttributes &attrs,
                                 QWidget *parent = nullptr);
    ~GifAttributesDialog();

private:
    Ui::GifAttributesDialog *ui;
    GifAttributes &m_attrs;

private slots:
    void accept() override;
    void onBrowseClicked();
};


#endif // GIFATTRIBUTESDIALOG_H
