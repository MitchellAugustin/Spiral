#include "textbox.h"

TextBox::TextBox(QWidget *parent, QString uuid) : QWidget(parent)
{
    this->uuid = uuid;
    qDebug() << "New TextBox object instantiated with UUID " << uuid;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 30, 0, 0);

    TextBox::richTextEdit->setText("Default text");

    DragResizeLabel *contractLabel = new DragResizeLabel(this, false);
    contractLabel->setText("<=");
    DragResizeLabel *expandLabel = new DragResizeLabel(this, true);
    expandLabel->setText("=>");
    TextBox::richTextEdit->f_toolbar->setVisible(false);
    TextBox::richTextEdit->f_toolbar_layout->addWidget(contractLabel);
    TextBox::richTextEdit->f_toolbar_layout->addWidget(expandLabel);
//    layout->addWidget(contractLabel);
//    layout->addWidget(expandLabel);
//    layout->setAlignment(contractLabel, Qt::AlignRight);
//    layout->setAlignment(expandLabel, Qt::AlignRight);
    layout->addWidget(TextBox::richTextEdit);
}
