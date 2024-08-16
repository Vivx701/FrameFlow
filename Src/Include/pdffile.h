#ifndef PDFFILE_H
#define PDFFILE_H

#include <QPdfWriter>
#include <QPainter>
#include <Types.h>
#include "IOutputFile.h"

class PdfFile : public IOutputFile
{
public:
    explicit PdfFile(QObject *parent=nullptr);
    void save() override;


};

#endif // PDFFILE_H
