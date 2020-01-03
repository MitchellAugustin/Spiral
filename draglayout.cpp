#include "draglayout.h"

#include <QtWidgets>

static inline QString spiralContentMimeType() { return QStringLiteral("application/x-spiralcontent"); }

static int getHeight(TextBox *tBox) {
    if(tBox->richTextEdit->toPlainText().isEmpty()) {
        delete tBox;
        return 0;
    }
    return tBox->richTextEdit->getHeight() + 30;
    QTextDocument *doc = tBox->richTextEdit->document();
    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    int h = 45;
    QTextBlock b = doc->begin();
    while (b != doc->end()) {
        h += layout->blockBoundingRect(b).height() + tBox->height();
        b = b.next();
    }
    return h;
}

DragLayout::DragLayout(QWidget *parent) : QWidget(parent)
{
//    int x = 5;
//    int y = 5;
//    TextBox *tBox = new TextBox(this);
//    tBox->move(x, y);
//    tBox->show();
//    tBox->setAttribute(Qt::WA_DeleteOnClose);
//    x += tBox->width() + 2;
//    if (x >= 245) {
//        x = 5;
//        y += tBox->height() + 2;
//    }

    setAcceptDrops(true);
}


void DragLayout::dragEnterEvent(QDragEnterEvent *event)
{
    //Handles movement of Spiral text boxes
    if (event->mimeData()->hasFormat(spiralContentMimeType())) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
      //Handles movement of plain text dropped into the Spiral window
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}


void DragLayout::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(spiralContentMimeType())) {
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


void DragLayout::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(spiralContentMimeType())) {
        const QMimeData *mime = event->mimeData();

        QByteArray itemData = mime->data(spiralContentMimeType());
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString text;
        QPoint offset;
        dataStream >> text >> offset;


        TextBox *tBox = new TextBox(this);
        tBox->richTextEdit->setText(text);
        tBox->move(event->pos() - offset);
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);
        tBox->resize(1500, getHeight(tBox));

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

    } else if (event->mimeData()->hasText()) {
        TextBox *tBox = new TextBox(this);
        tBox->richTextEdit->setText(event->mimeData()->text());
        tBox->move(event->pos());
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);
        tBox->resize(1500, getHeight(tBox));

        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}



void DragLayout::mousePressEvent(QMouseEvent *event)
{
    TextBox *child = static_cast<TextBox*>(childAt(event->pos()));
    if (!child) {
        TextBox *tBox = new TextBox(this);
        tBox->richTextEdit->setText("Type here");
        tBox->move(event->pos());
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);
        tBox->resize(1500, getHeight(tBox));
        return;
    }

    QPoint hotSpot = event->pos() - child->pos();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << child->richTextEdit->toHtml() << QPoint(hotSpot);

    QByteArray htmlItemData;
    QDataStream htmlDataStream(&htmlItemData, QIODevice::WriteOnly);
    htmlDataStream << child->richTextEdit->toHtml();

    QByteArray plainTextItemData;
    QDataStream plainTextDataStream(&plainTextItemData, QIODevice::WriteOnly);
    plainTextDataStream << child->richTextEdit->toPlainText();

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(spiralContentMimeType(), itemData);
    mimeData->setData("text/html", htmlItemData);
    mimeData->setData("text/plain", plainTextItemData);


    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(hotSpot);

    child->hide();


    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        child->close();
    else
        child->show();
}
