#include "dragresizelabel.h"
#include "textbox.h"

DragResizeLabel::DragResizeLabel(QWidget *parent, bool expand)
{
    DragResizeLabel::expand = expand;
    setAcceptDrops(true);
}

void DragResizeLabel::dragEnterEvent(QDragEnterEvent *event)
{
    QWidget *parent = parentWidget();
    TextBox *textBox = dynamic_cast<TextBox*>(parent->parentWidget()->parentWidget());
    textBox->resize(textBox->width() + (expand ? 20 : -20), textBox->height());
    qDebug() << "TextBox " << (expand ? "expanded" : "contracted");
}

void DragResizeLabel::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "dragMoveEvent";
}

void DragResizeLabel::dropEvent(QDropEvent *event)
{
    qDebug() << "dropEvent";
}

void DragResizeLabel::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent";
    QWidget *parent = parentWidget();
    TextBox *textBox = dynamic_cast<TextBox*>(parent->parentWidget()->parentWidget());

//    qDebug() << "Parent: " << parent;
//    qDebug() << "TextBox: " << textBox;
//    textBox->resize(textBox->width() + 20, textBox->height());


    QPoint hotSpot = event->pos() - parent->pos();

    QMimeData *mimeData = new QMimeData;
    //Set the location of the TextBox as the MIME data so it can be resized from DragLayout when received
    QByteArray plainTextItemData;
    QDataStream plainTextDataStream(&plainTextItemData, QIODevice::WriteOnly);
    plainTextDataStream << textBox->pos().rx() << "," << textBox->pos().ry();

    mimeData->setData(RESIZE_MIME_TYPE, plainTextItemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(hotSpot);

    drag->exec();
}
