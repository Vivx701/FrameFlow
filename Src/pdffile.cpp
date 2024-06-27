#include "pdffile.h"

/**
 * @brief Constructor for the PdfFile class.
 * @param parent The parent object.
 */
PdfFile::PdfFile(QObject *parent): IOutputFile(parent)
{
    m_Attrib = PDFAttributes();
}

/**
 * @brief Save the images as a PDF file with different fill styles on each page.
 *
 * This function saves the images stored in the `m_Images` member variable as a PDF file.
 * Each image is drawn on a separate page with a different fill style.
 * The fill styles cycle through the following options:
 * - Center: The image is centered on the page without scaling.
 * - Fill: The image is scaled to fill the entire page, potentially cropping the image.
 * - Fit: The image is scaled to fit within the page while maintaining its aspect ratio.
 *
 * The function uses the `PDFAttributes` stored in the `m_Attrib` member variable to set
 * various properties of the PDF file, such as the file path, page size, creator, margins,
 * orientation, PDF version, title, and background color.
 *
 * The function emits the `progressChanged` signal to indicate the progress of saving the images.
 */
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


    // Determine the fill style for the current page
    QString fillStyle = attrib->specificSettings["Fill"].toString();;


    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBackgroundMode(Qt::OpaqueMode);

    int count = 0;
    emit progressChanged(m_Images.count(), count);
    for (QImage image: m_Images){
        painter.fillRect(painter.viewport(),  attrib->background);

        QRectF pageRect = painter.viewport();
        QRectF imageRect = image.rect();

        QRectF targetRect;
        if (fillStyle == "Center") {
            // Center the image on the page without scaling
            targetRect = imageRect;
            targetRect.moveCenter(pageRect.center());
        } else if (fillStyle == "Fill") {
            // Scale the image to fill the entire page, potentially cropping the image
            double scaleFactorWidth = pageRect.width() / imageRect.width();
            double scaleFactorHeight = pageRect.height() / imageRect.height();
            double scaleFactor = qMax(scaleFactorWidth, scaleFactorHeight);
            targetRect = QRectF(0, 0, imageRect.width() * scaleFactor, imageRect.height() * scaleFactor);
            targetRect.moveCenter(pageRect.center());
        } else if (fillStyle == "Fit") {
            // Scale the image to fit within the page while maintaining the aspect ratio
            double scaleFactorWidth = pageRect.width() / imageRect.width();
            double scaleFactorHeight = pageRect.height() / imageRect.height();
            double scaleFactor = qMin(scaleFactorWidth, scaleFactorHeight);
            targetRect = QRectF(0, 0, imageRect.width() * scaleFactor, imageRect.height() * scaleFactor);
            targetRect.moveCenter(pageRect.center());
        }

        painter.drawImage(targetRect, image);
        pdfWriter.newPage();
        count++;
        emit progressChanged(m_Images.count(), count);
    }
    painter.end();
}
