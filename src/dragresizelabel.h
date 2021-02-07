/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef DRAGRESIZELABEL_H
#define DRAGRESIZELABEL_H

#include <QLabel>
#include <QtWidgets>
#include "constants.h"

class TextBox;

/**
 * @brief DragResizeLabel - Handles textbox resizing
 * @author Mitchell Augustin - https://mitchellaugustin.com
 * @param parent - The parent widget (should be the resizer's corresponding TextBox)
 * @param expand - True if this label is the expand label, false if it contracts
 */
class DragResizeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DragResizeLabel(TextBox *parent = nullptr, bool expand = true);
    void parentTextBoxDeleted() {
        parentTextBox = nullptr;
        close();
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool expand;
    TextBox *parentTextBox;
};

#endif //DRAGRESIZELABEL_H
