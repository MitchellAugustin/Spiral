#include "dragresizelabel.h"
#include "textbox.h"

DragResizeLabel::DragResizeLabel(TextBox *parent, bool expand)
{
    DragResizeLabel::expand = expand;
    this->parentTextBox = parent;
    setAcceptDrops(true);
}

void DragResizeLabel::dragEnterEvent(QDragEnterEvent *event)
{
    //No implementation needed
}

void DragResizeLabel::dragMoveEvent(QDragMoveEvent *event)
{
    //No implementation needed
}

void DragResizeLabel::dropEvent(QDropEvent *event)
{
    //No implementation needed
}

void DragResizeLabel::mousePressEvent(QMouseEvent *event)
{
    TextBox *textBox = parentTextBox;
    if (textBox) {
        textBox->resize(textBox->width() + (expand ? 20 : -20), textBox->height());
        qDebug() << "TextBox" << (expand ? "expanded" : "contracted");
    }
}
