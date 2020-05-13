#include "draglayout.h"

#include <QtWidgets>

static inline QString spiralContentMimeType() { return SPIRAL_CONTENT_MIME_TYPE; }

static int getHeight(TextBox *tBox) {
    if(tBox->richTextEdit->toPlainText().isEmpty()) {
        DragLayout *parentDrag = (DragLayout*) tBox->parentWidget();
//        parentDrag->parentPage->textBoxList.remove(tBox->thisBoxIndex);
        parentDrag->parentPage->textBoxList.removeOne(tBox);
        tBox->close();
        qDebug() << "TextBox " << tBox->uuid << " deleted from getHeight in draglayout";
        return 0;
    }
    return utilities::getMRichTextEditHeight(tBox->richTextEdit);
}

DragLayout::DragLayout(QWidget *parent, Page *parentPage) : QWidget(parent)
{
    setAcceptDrops(true);
    this->parentPage = parentPage;
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

        //Dynamically resize the DragLayout area
        if(event->pos().x() + DEFAULT_TEXTBOX_WIDTH > this->width()) {
            this->resize(this->width() + event->pos().x() + DEFAULT_TEXTBOX_WIDTH, this->height());
        }
        //Note: This uses DEFAULT_WIDTH for the movement calculation so the box can be moved, but the page must be resized again
        //on drop so the actual TextBox size can be used in the calculation.
        if(event->pos().y() + DEFAULT_TEXTBOX_WIDTH > this->height()) {
            this->resize(this->width(), this->height() + event->pos().y() + DEFAULT_TEXTBOX_WIDTH);
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
        QString uuid;
        int width;
        dataStream >> text >> offset >> uuid >> width;

        qDebug() << "Width from data stream: " << width;

        TextBox *tBox = new TextBox(this, uuid);
        parentPage->textBoxList.append(tBox);
        qDebug() << "Appended in dropEvent1";
        tBox->richTextEdit->setText(text);
        tBox->move(event->pos() - offset);
        tBox->location = event->pos();
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);
        tBox->resize(width, getHeight(tBox));

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

        //Finalize resize operation
        if(event->pos().y() + getHeight(tBox) > this->height()) {
            this->resize(this->width(), this->height() + event->pos().y() + getHeight(tBox));
        }
        qDebug() << "TextBox " << tBox->uuid << " moved to " << event->pos();
    } else if (event->mimeData()->hasText()) {
        TextBox *tBox = new TextBox(this);
        parentPage->textBoxList.append(tBox);
        qDebug() << "Appended in dropEvent2";
        tBox->richTextEdit->setText(event->mimeData()->text());
        tBox->move(event->pos());
        tBox->location = event->pos();
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);
        tBox->resize(DEFAULT_TEXTBOX_WIDTH, getHeight(tBox));

        event->acceptProposedAction();
        //Finalize resize operation
        if(event->pos().y() + getHeight(tBox) > this->height()) {
            this->resize(this->width(), this->height() + event->pos().y() + getHeight(tBox));
        }
        qDebug() << "TextBox moved to " << event->pos();
    } else if (event->mimeData()->hasFormat(RESIZE_MIME_TYPE)) {
        qDebug() << "Resize";
        qDebug() << "Object to resize located at " << event->mimeData()->text();
    } else {
        qDebug() << "Ignoring event with type " << event->mimeData()->formats();
        event->ignore();
    }
}

void DragLayout::newTextBoxAtLocation(QPoint point) {
    TextBox *tBox = new TextBox(this);
    parentPage->textBoxList.append(tBox);
    qDebug() << "Appended in newTextBoxAtLocation";
    tBox->richTextEdit->setText("Added programatically");
    tBox->move(point);
    tBox->location = point;
    tBox->show();
    tBox->setAttribute(Qt::WA_DeleteOnClose);
    tBox->resize(DEFAULT_TEXTBOX_WIDTH, getHeight(tBox));
    return;
}


void DragLayout::mousePressEvent(QMouseEvent *event)
{
    TextBox *child = dynamic_cast<TextBox*>(childAt(event->pos()));
    QFrame *childQFrameCheck = dynamic_cast<QFrame*>(childAt(event->pos()));
    MRichTextEdit *childMRichTextEditCheck = dynamic_cast<MRichTextEdit*>(childAt(event->pos()));

//    qDebug() << "child: " << child;
//    qDebug() << "childQFrame: " << childQFrameCheck;
//    qDebug() << "childMText: " << childMRichTextEditCheck;

    //Ensures that drag operations are not handled unless the user is dragging a TextBox object
    bool childInnerCheck = child && !(childQFrameCheck || childMRichTextEditCheck);

    if (!child && !childInnerCheck) {
        TextBox *tBox = new TextBox(this);
        parentPage->textBoxList.append(tBox);
        qDebug() << "Appended in mousePressEvent";
        tBox->richTextEdit->setText("Type here");
        tBox->move(event->pos());
        tBox->location = event->pos();
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);
        tBox->resize(DEFAULT_TEXTBOX_WIDTH, getHeight(tBox));
        event->accept();
        return;
    }

    //If the user is dragging a TextBox object by its draggable area and NOT touching an inner layout component
//    qDebug() << "childInnerCheck: " << childInnerCheck;
    if (child && childInnerCheck) {
        QPoint hotSpot = event->pos() - child->pos();

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << child->richTextEdit->toHtml() << QPoint(hotSpot) << child->uuid << child->width();

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


        if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction) {
            qDebug() << "TextBox removed from list in mousePressEvent";
//            parentPage->textBoxList.remove(child->thisBoxIndex);
            parentPage->textBoxList.removeOne(child);
            child->close();
        }
        else
            child->show();
    }
}
