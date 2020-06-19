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

class TextBox : public QFrame
{
    Q_OBJECT
public:
    TextBox(QWidget *parent = nullptr, QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]);
    ~TextBox() {
        if(contractLabel) {
            contractLabel->parentTextBoxDeleted();
        }
        if(expandLabel) {
            expandLabel->parentTextBoxDeleted();
        }
    }

    QString uuid;
    MRichTextEdit* richTextEdit = new MRichTextEdit(this);
    QPoint location;
    DragResizeLabel *contractLabel = new DragResizeLabel(this, false);
    DragResizeLabel *expandLabel = new DragResizeLabel(this, true);
};

#endif // TEXTBOX_H
