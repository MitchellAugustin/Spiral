#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "spiralcomponent.h"
#include "section.h"

class Notebook : public SpiralComponent
{
public:
    Notebook(ResourceHandler *resourceHandler) : SpiralComponent(resourceHandler) {
        //ResourceHandler is set in SpiralComponent superclass
    }
    const QString class_name = "Notebook";
    QVector<Section> sectionsList = {};
    QString path = "";
};

#endif // NOTEBOOK_H
