#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

class textbox : public QWidget
{
    Q_OBJECT
public:
    textbox(QWidget *parent = nullptr);
    ~textbox() {}
signals:
    void updateFile();
};

#endif // TEXTBOX_H
