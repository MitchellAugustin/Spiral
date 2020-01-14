#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>

#include "constants.h"
#include "mrichtextedit.h"
#include "dragresizelabel.h"

class TextBox : public QWidget
{
    Q_OBJECT
public:
    TextBox(QWidget *parent = nullptr);
    ~TextBox() {}
    MRichTextEdit* richTextEdit = new MRichTextEdit(this);
};

#endif // TEXTBOX_H
