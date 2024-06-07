#include "pdffile.h"

PdfFile::PdfFile(QObject *parent): IOutputFile(parent)
{

}

void PdfFile::save(const QString &filePath)
{
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(10, 10, 10, 10));

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);
    for (QImage image: m_Images){
        painter.drawImage(painter.viewport(), image);
        pdfWriter.newPage();
    }

    painter.end();
}
