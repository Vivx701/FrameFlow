#include "imagedelegate.h"

#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

/**
 * @brief Constructor for ImageDelegate.
 * @param parent The parent object.
 */
ImageDelegate::ImageDelegate(QObject* parent) : QStyledItemDelegate(parent) {

}

/**
 * @brief Overridden paint function to render the image and caption in each cell.
 * @param painter The painter used for rendering.
 * @param option The style option for the cell.
 * @param index The model index of the cell.
 */
void ImageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QImage previewImg = index.data(PreviewRole).value<QImage>();
    if (!previewImg.isNull()) {


        QString caption = index.data(FileNameRole).toString();

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);


        // Draw the image
        QRect imageRect = option.rect.adjusted(5, 5, -5, -25);
        painter->drawImage(imageRect, previewImg);
        painter->drawRect(option.rect);

        // Draw the caption
        QFontMetrics fontMetrics = painter->fontMetrics();

        QRect captionRect = option.rect.adjusted(5, option.rect.height() - 20, -5, -5);
        QString elidedText = fontMetrics.elidedText(caption, Qt::ElideRight, captionRect.width());
        painter->drawText(captionRect, Qt::AlignHCenter | Qt::AlignVCenter, caption);

        // Draw the delete button
        QStyleOptionButton buttonOption;
        buttonOption.rect = QRect(option.rect.right() - 30, option.rect.top() + 20, 20, 20);
        buttonOption.state = QStyle::State_Enabled;
        buttonOption.text  = "X";

        QStyle* style = option.widget ? option.widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_PushButton, &buttonOption, painter, option.widget);

        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

/**
 * @brief Overridden editorEvent function to handle the delete button click.
 * @param event The event that occurred.
 * @param model The model associated with the delegate.
 * @param option The style option for the cell.
 * @param index The model index of the cell.
 * @return True if the event was handled, false otherwise.
 */
bool ImageDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QRect buttonRect = QRect(option.rect.right() - 25, option.rect.top() + 5, 20, 20);

        if (buttonRect.contains(mouseEvent->pos())) {
            emit deleteButtonClicked(index);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

/**
 * @brief Overridden sizeHint function to specify the size of each cell.
 * @param option The style option for the cell.
 * @param index The model index of the cell.
 * @return The size hint for the cell.
 */
QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);

    return index.data(Qt::SizeHintRole).value<QSize>();
}
