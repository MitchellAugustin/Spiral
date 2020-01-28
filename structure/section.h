#ifndef SECTION_H
#define SECTION_H

#include <QVector>
#include <QString>
#include <QStringList>

#include "spiralcomponent.h"
#include "page.h"

class Section : public SpiralComponent
{
public:
    Section(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) : SpiralComponent(uuid) {
        pagesList = new QVector<Page*>();
        qDebug() << "Section instantiated with UUID " << uuid;
    }
    void addPage(Page *page) {
        pagesList->append(page);
        //Add page to UI
    }
    QVector<Page*> *loadPagesList() {
        return pagesList;
    }
    const QString class_name = "Section";

private:
    QVector<Page*> *pagesList = {};
};

#endif // SECTION_H
