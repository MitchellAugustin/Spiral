#ifndef PAGE_H
#define PAGE_H

#include <QVector>

#include "spiralcomponent.h"
#include "draglayout.h"

class Page : public SpiralComponent
{
public:
    Page(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) : SpiralComponent(uuid) {
        qDebug() << "Page instantiated with UUID " << uuid;
    }
    const QString class_name = "Page";

    QWidget *dragLayout = nullptr;
    QVector<TextBox*> textBoxList;
};

#endif // PAGE_H
