/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>
#include <QUuid>
#include <QTime>

#include "constants.h"
#include "mrichtextedit.h"
#include "dragresizelabel.h"

/**
 * @brief TextBox - Container class for textboxes
 * @author Mitchell Augustin - https://mitchellaugustin.com
 * @param parent - The parent widget of this TextBox
 * @param uuid - This TextBox's UUID
 */
class TextBox : public QFrame
{
    Q_OBJECT
public:
    TextBox(QWidget *parent = nullptr, bool *queryUpdated = nullptr, QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]);
    ~TextBox() {
        if(contractLabel) {
            contractLabel->parentTextBoxDeleted();
        }
        if(expandLabel) {
            expandLabel->parentTextBoxDeleted();
        }
        richTextEdit->f_toolbar->setEnabled(false);
    }

    QString uuid;
    bool *queryUpdated;
    MRichTextEdit* richTextEdit;
    QPoint location;
    DragResizeLabel *contractLabel = new DragResizeLabel(this, false);
    DragResizeLabel *expandLabel = new DragResizeLabel(this, true);
};

#endif // TEXTBOX_H
