#ifndef PAGE_H
#define PAGE_H

#include <QVector>

#include "spiralcomponent.h"
#include "textbox.h"

class Page : public SpiralComponent
{
public:
    Page(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) : SpiralComponent(uuid) {
        qDebug() << "Page instantiated with UUID " << uuid;
    }
    ~Page() {
        delete editorPane;
        delete dragLayout;
    }
    const QString class_name = "Page";

    QWidget *editorPane = nullptr;
    QWidget *dragLayout = nullptr;
    QVector<TextBox*> textBoxList;
};

#endif // PAGE_H
