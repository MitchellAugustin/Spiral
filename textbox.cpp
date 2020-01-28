#include "textbox.h"

TextBox::TextBox(QWidget *parent, QString uuid) : QWidget(parent)
{
    this->uuid = uuid;
    qDebug() << "New TextBox object instantiated with UUID " << uuid;
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 30, 0, 0);

    TextBox::richTextEdit->setText("Default text");
    layout->addWidget(TextBox::richTextEdit);

    DragResizeLabel *contractLabel = new DragResizeLabel(this, false);
    contractLabel->setText("<=");
    DragResizeLabel *expandLabel = new DragResizeLabel(this, true);
    expandLabel->setText("=>");
    TextBox::richTextEdit->f_toolbar_layout->addWidget(contractLabel);
    TextBox::richTextEdit->f_toolbar_layout->addWidget(expandLabel);
}
