/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include "dragresizelabel.h"
#include "textbox.h"

/**
 * @brief dragresizelabel.cpp - Handles textbox resizing
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */


/**
 * @brief DragResizeLabel::DragResizeLabel - Handles textbox resizing
 * @param parent - The parent widget (should be the resizer's corresponding TextBox)
 * @param expand - True if this label is the expand label, false if it contracts
 */
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

/**
 * @brief DragResizeLabel::mousePressEvent - Handles expansion/contraction of the corresponding TextBox
 * @param event
 */
void DragResizeLabel::mousePressEvent(QMouseEvent *event)
{
    TextBox *textBox = parentTextBox;
    if (textBox) {
        textBox->resize(textBox->width() + (expand ? 20 : -20), textBox->height());
        qDebug() << "TextBox" << (expand ? "expanded" : "contracted");
    }
}
