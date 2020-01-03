#include "textbox.h"

TextBox::TextBox(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 30, 0, 0);

    TextBox::richTextEdit->setText("Default text");
    layout->addWidget(TextBox::richTextEdit);
}
