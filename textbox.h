#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

#include "mrichtextedit.h"

class textbox : public QWidget
{
    Q_OBJECT
public:
    textbox(QWidget *parent = nullptr);
    ~textbox() {}
    MRichTextEdit* richTextEdit = new MRichTextEdit(this);
signals:
    void updateFile();
};

#endif // TEXTBOX_H
