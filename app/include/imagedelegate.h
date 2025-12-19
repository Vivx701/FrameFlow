#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QFontMetrics>
#include <Types.h>

/**
 * @class ImageDelegate
 * @brief A custom delegate for rendering images and captions in a QTableView.
 */
class ImageDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    /**
     * @brief Constructor for ImageDelegate.
     * @param parent The parent object.
     */
    ImageDelegate(QObject* parent = nullptr);

    /**
     * @brief Overridden paint function to render the image and caption in each cell.
     * @param painter The painter used for rendering.
     * @param option The style option for the cell.
     * @param index The model index of the cell.
     */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /**
     * @brief Overridden editorEvent function to handle the delete button click.
     * @param event The event that occurred.
     * @param model The model associated with the delegate.
     * @param option The style option for the cell.
     * @param index The model index of the cell.
     * @return True if the event was handled, false otherwise.
     */
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

    /**
     * @brief Overridden sizeHint function to specify the size of each cell.
     * @param option The style option for the cell.
     * @param index The model index of the cell.
     * @return The size hint for the cell.
     */
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

signals:
    /**
     * @brief Signal emitted when the delete button is clicked.
     * @param index The model index of the cell containing the clicked delete button.
     */
    void deleteButtonClicked(const QModelIndex& index);
};

#endif // IMAGEDELEGATE_H
