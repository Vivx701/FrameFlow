#include "pdffile.h"

PdfFile::PdfFile(QObject *parent): IOutputFile(parent)
{
    m_Attrib = PDFAttributes();
}

void PdfFile::save()
{
    PDFAttributes *attrib = static_cast<PDFAttributes*>(&m_Attrib);
    if(attrib->filePath.isEmpty())
    {
        return;
    }

    QPdfWriter pdfWriter(attrib->filePath);

    QPageSize pageSize = qvariant_cast<QPageSize>(attrib->specificSettings["Size"]);
    pdfWriter.setPageSize(pageSize);

    QString creator = attrib->specificSettings["Creator"].toString();
    pdfWriter.setCreator(creator);

    QMarginsF margin = qvariant_cast<QMarginsF>(attrib->specificSettings["Margin"]);
    pdfWriter.setPageMargins(margin);

    QPageLayout::Orientation orientation = qvariant_cast<QPageLayout::Orientation>(attrib->specificSettings["Orientation"]);
    pdfWriter.setPageOrientation(orientation);

    QPagedPaintDevice::PdfVersion pdfVersion = qvariant_cast<QPagedPaintDevice::PdfVersion>(attrib->specificSettings["Version"]);
    pdfWriter.setPdfVersion(pdfVersion);

    QString title = attrib->specificSettings["Title"].toString();
    pdfWriter.setTitle(title);


    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBackgroundMode(Qt::OpaqueMode);

    int count = 0;
    emit progressChanged(m_Images.count(), count);
    for (QImage image: m_Images){
        painter.fillRect(painter.viewport(),  attrib->background);
        painter.drawImage(painter.viewport(), image);
        pdfWriter.newPage();
        count++;
        emit progressChanged(m_Images.count(), count);
    }
    painter.end();
}
