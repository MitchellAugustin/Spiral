#include "draglayout.h"

#include <QtWidgets>

static inline QString fridgetMagnetsMimeType() { return QStringLiteral("application/x-fridgemagnet"); }

static textbox *createNewTextbox(const QString &htmlText, QWidget *parent) {
    textbox *tBox = new textbox(parent);
    tBox->richTextEdit->setHtml(htmlText);
    return tBox;
}
//! [0]
DragLayout::DragLayout(QWidget *parent)
    : QWidget(parent)
{
    int x = 5;
    int y = 5;
    textbox *tBox = new textbox(this);
    tBox->move(x, y);
    tBox->show();
    tBox->setAttribute(Qt::WA_DeleteOnClose);
    x += tBox->width() + 2;
    if (x >= 245) {
        x = 5;
        y += tBox->height() + 2;
    }

//! [2] //! [3]
    setAcceptDrops(true);
}
//! [3]

//! [4]
void DragLayout::dragEnterEvent(QDragEnterEvent *event)
{
//! [4] //! [5]
    if (event->mimeData()->hasFormat(fridgetMagnetsMimeType())) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
//! [5] //! [6]
        }
//! [6] //! [7]
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
//! [7]

//! [8]
void DragLayout::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(fridgetMagnetsMimeType())) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
//! [8]

//! [9]
void DragLayout::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(fridgetMagnetsMimeType())) {
        const QMimeData *mime = event->mimeData();
//! [9] //! [10]
        QByteArray itemData = mime->data(fridgetMagnetsMimeType());
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString text;
        QPoint offset;
        dataStream >> text >> offset;
//! [10]
//! [11]
        textbox *tBox = new textbox(this);
        tBox->richTextEdit->setText(text);
        tBox->move(event->pos() - offset);
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
//! [11] //! [12]
    } else if (event->mimeData()->hasText()) {
        textbox *tBox = new textbox(this);
        tBox->richTextEdit->setText(event->mimeData()->text());
        tBox->move(event->pos());
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);

        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
//! [12]

//! [13]
void DragLayout::mousePressEvent(QMouseEvent *event)
{
//! [13]
//! [14]
    textbox *child = static_cast<textbox*>(childAt(event->pos()));
    if (!child)
        return;

    QPoint hotSpot = event->pos() - child->pos();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << child->richTextEdit->toHtml() << QPoint(hotSpot);
//! [14]

//! [15]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(fridgetMagnetsMimeType(), itemData);
    mimeData->setText(child->richTextEdit->toHtml());
//! [15]

//! [16]
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(hotSpot);

    child->hide();
//! [16]

//! [17]
    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        child->close();
    else
        child->show();
}
//! [17]
