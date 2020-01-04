#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>

#include "mrichtextedit.h"

class TextBox : public QWidget
{
    Q_OBJECT
public:
    TextBox(QWidget *parent = nullptr);
    ~TextBox() {}
    MRichTextEdit* richTextEdit = new MRichTextEdit(this);
};

#endif // TEXTBOX_H
