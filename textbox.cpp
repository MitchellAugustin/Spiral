#include "textbox.h"
textbox::textbox(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 30, 0, 0);

    QTextEdit *textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);
}
