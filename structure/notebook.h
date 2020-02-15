#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "spiralcomponent.h"
#include "section.h"

class Notebook : public SpiralComponent
{
public:
    Notebook(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) : SpiralComponent(uuid) {
        sectionsList = new QVector<Section*>();
        qDebug() << "Notebook instantiated with UUID " << uuid;
    }
    void addSection(Section *section) {
        sectionsList->append(section);
        //Add section to UI
    }
    QVector<Section*> *loadSectionsList() {
        return sectionsList;
    }
    const QString class_name = "Notebook";
    QString path = "";
private:
    QVector<Section*> *sectionsList = {};
};

#endif // NOTEBOOK_H