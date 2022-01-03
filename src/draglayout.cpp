/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "draglayout.h"

#include <QtWidgets>


/**
 * @brief draglayout.cpp - Handles all user actions within a Page's draggable region (where all TextBox objects are placed)
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */


/**
 * @brief spiralContentMimeType - Returns the MIME type of Spiral textboxes
 * @return
 */
static inline QString spiralContentMimeType() { return SPIRAL_CONTENT_MIME_TYPE; }

/**
 * @brief getHeight - Returns the height of the parameterized TextBox and closes it if empty
 * @param tBox
 * @return
 */
int DragLayout::getHeight(TextBox *tBox) {
    if(tBox->richTextEdit->toPlainText().isEmpty()) {
        DragLayout *parentDrag = static_cast<DragLayout*> (tBox->parentWidget());
        parentDrag->parentPage->textBoxList.removeOne(tBox);
        tBox->close();
        qDebug() << "TextBox " << tBox->uuid << " deleted from getHeight in draglayout";
        (*queryUpdated) = false;
        return 0;
    }
    return utilities::getMRichTextEditHeight(tBox->richTextEdit);
}

/**
 * @brief DragLayout::DragLayout - Initializes a new DragLayout()
 * @param parent
 * @param parentPage
 */
DragLayout::DragLayout(QWidget *parent, Page *parentPage, bool *queryUpdated) : QWidget(parent)
{
    setAcceptDrops(true);
    this->parentPage = parentPage;
    this->queryUpdated = queryUpdated;
}


/**
 * @brief DragLayout::dragEnterEvent - Handles this DragLayout's QDragEnterEvents
 * @param event
 */
void DragLayout::dragEnterEvent(QDragEnterEvent *event)
{
    (*queryUpdated) = false;
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


/**
 * @brief DragLayout::dragMoveEvent - Handles this DragLayout's QDragMoveEvents
 * @param event
 */
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

/**
 * @brief DragLayout::dropEvent - Handles this DragLayout's QDropEvents
 * @param event
 */
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

        TextBox *tBox = new TextBox(this, queryUpdated, uuid);
        parentPage->textBoxList.append(tBox);
        qDebug() << "Appended in dropEvent1";
        tBox->richTextEdit->setText(text);
        tBox->move(event->pos() - offset);
        tBox->location = event->pos();
        tBox->show();
        tBox->setAttribute(Qt::WA_DeleteOnClose);
        int tHeight = getHeight(tBox);
        qDebug() << "dropevent1 height:" << tHeight;
        tBox->resize(width, tHeight);

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

        tBox->resize(width, getHeight(tBox));
        qDebug() << "Final dropevent1 height:" << tHeight;

        //Finalize resize operation (Resizes canvas if necessary)
        if(event->pos().y() + getHeight(tBox) > this->height()) {
            this->resize(this->width(), this->height() + event->pos().y() + getHeight(tBox));
        }
        qDebug() << "TextBox " << tBox->uuid << " moved to " << event->pos();
    } else if (event->mimeData()->hasText()) {
        TextBox *tBox = new TextBox(this, queryUpdated);
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

/**
 * @brief DragLayout::newTextBoxAtLocation - Programmatically inserts a new TextBox at the parameterized point
 * Note: This is mainly a proof-of-concept to show that this can be done upon file load
 * @param point
 */
TextBox *DragLayout::newTextBoxAtLocation(QString uuid, QPoint point, int width, QString content) {
    TextBox *tBox = new TextBox(this, queryUpdated);
    parentPage->textBoxList.append(tBox);
    qDebug() << "Appended in newTextBoxAtLocation";
    if (content != nullptr) {
        tBox->richTextEdit->setHtml(content);
    }
    else {
        tBox->richTextEdit->setText("Added programatically");
    }

    if (uuid != nullptr) {
        tBox->uuid = uuid;
    }
    tBox->move(point);
    tBox->location = point;
    tBox->show();
    tBox->setAttribute(Qt::WA_DeleteOnClose);
    tBox->resize(width, getHeight(tBox));
    //Dynamically resize the DragLayout area
    if(point.x() + tBox->width() + DEFAULT_TEXTBOX_WIDTH > this->width()) {
        this->resize(this->width() + point.x() + tBox->width() + DEFAULT_TEXTBOX_WIDTH, this->height());
    }
    //Note: This uses DEFAULT_WIDTH for the movement calculation so the box can be moved, but the page must be resized again
    //on drop so the actual TextBox size can be used in the calculation.
    if(getHeight(tBox) > this->height()) {
        this->resize(this->width(), this->height() + getHeight(tBox));
    }
    tBox->resize(tBox->width(), tBox->height());
    return tBox;
}

/**
 * @brief DragLayout::mousePressEvent - Handles this DragLayout's QMouseEvents
 * @param event
 */
void DragLayout::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        TextBox *child = dynamic_cast<TextBox*>(childAt(event->pos()));
        // QFrame *childQFrameCheck = dynamic_cast<QFrame*>(childAt(event->pos()));
        MRichTextEdit *childMRichTextEditCheck = dynamic_cast<MRichTextEdit*>(childAt(event->pos()));

        //Ensures that drag operations are not handled unless the user is dragging a TextBox object
        bool childInnerCheck = child && !(childMRichTextEditCheck);

        if (!child && !childInnerCheck) {
            TextBox *tBox = new TextBox(this, queryUpdated);
            parentPage->textBoxList.append(tBox);
            qDebug() << "Appended in mousePressEvent";
            tBox->richTextEdit->setText("Type here");
            tBox->move(event->pos());
            tBox->location = event->pos();
            tBox->show();
            tBox->setAttribute(Qt::WA_DeleteOnClose);
            tBox->resize(DEFAULT_TEXTBOX_WIDTH, getHeight(tBox));
            event->accept();
            tBox->richTextEdit->f_textedit->setFocus();
            tBox->richTextEdit->f_textedit->setTextCursor(tBox->richTextEdit->f_textedit->document()->find("Type here", 0, QTextDocument::FindCaseSensitively));
            return;
        }

        //If the user is dragging a TextBox object by its draggable area and NOT touching an inner layout component
        if (child && childInnerCheck) {
            QPoint hotSpot = event->pos() - child->pos();

            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream << child->richTextEdit->toHtml() << hotSpot << child->uuid << child->width();

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
            drag->setPixmap(child->grab());

            child->hide();


            if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction) {
                qDebug() << "TextBox removed from list in mousePressEvent";
                parentPage->textBoxList.removeOne(child);
                child->close();
            }
            else
                child->show();
        }
    }
}
