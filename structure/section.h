#ifndef SECTION_H
#define SECTION_H

#include <QVector>

#include "spiralcomponent.h"
#include "page.h"

class Section : public SpiralComponent
{
public:
    Section(ResourceHandler *resourceHandler) : SpiralComponent(resourceHandler) {
        //ResourceHandler is set in superclass
    }
    const QString class_name = "Section";
    QVector<Page> pagesList = {};
};

#endif // SECTION_H
