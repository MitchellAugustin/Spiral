/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "textbox.h"

/**
 * @brief TextBox - Container class for textboxes
 * @author Mitchell Augustin - https://mitchellaugustin.com
 * @param parent - The parent widget of this TextBox
 * @param uuid - This TextBox's UUID
 */
TextBox::TextBox(QWidget *parent, QString uuid) : QFrame(parent)
{
    this->uuid = uuid;
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    qDebug() << "New TextBox object instantiated with UUID " << uuid;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 15, 0, 0);

    TextBox::richTextEdit->setText("Default text");

    contractLabel->setText("<=");
    expandLabel->setText("=>");
    TextBox::richTextEdit->f_toolbar->setVisible(false);
    TextBox::richTextEdit->f_toolbar_layout->addWidget(contractLabel);
    TextBox::richTextEdit->f_toolbar_layout->addWidget(expandLabel);
    TextBox::richTextEdit->f_toolbar->setFocusPolicy(Qt::NoFocus);
//    layout->addWidget(contractLabel);
//    layout->addWidget(expandLabel);
//    layout->setAlignment(contractLabel, Qt::AlignRight);
//    layout->setAlignment(expandLabel, Qt::AlignRight);
    layout->addWidget(TextBox::richTextEdit);
}
