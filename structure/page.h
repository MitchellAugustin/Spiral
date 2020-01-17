#ifndef PAGE_H
#define PAGE_H

#include <QVector>

#include "spiralcomponent.h"


class Page : public SpiralComponent
{
public:
    Page(ResourceHandler *resourceHandler) : SpiralComponent(resourceHandler) {
        //ResourceHandler is set in SpiralComponent superclass
    }
    const QString class_name = "Page";
    //TBD: Collection of page elements (representing TextBox content)
};

#endif // PAGE_H
