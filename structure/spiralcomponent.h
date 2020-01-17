#ifndef SPIRALCOMPONENT_H
#define SPIRALCOMPONENT_H

#include <QString>
#include <QVector>

#include "resourcehandler.h"

class SpiralComponent
{
public:
    SpiralComponent(ResourceHandler *resourceHandler) {
        this->resourceHandler = resourceHandler;
    }
    const QString class_name = "SpiralComponent";
    /** For the time being, I am going to provide a UUID variable,
     * but since everything will be saved in one file, it may not be necessary.
     * */
    const QString uuid = "NOUUID";
    ResourceHandler *resourceHandler;
};

#endif // SPIRALCOMPONENT_H

/**
 *     public static final String CLASS_NAME = "Notebook";
    private static int sectionsInNotebook = 0;

    private ResourceHandler resourceHandler;
    private String name;
    private UUID uuid;
    private ArrayList<NotebookSection> sections;
    private String path;
    */
