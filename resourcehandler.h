#ifndef RESOURCEHANDLER_H
#define RESOURCEHANDLER_H

#include <QVector>

class ResourceHandler;

#include "structure/notebook.h"

class ResourceHandler
{
public:
    ResourceHandler();
    //This vector will contain all currently open notebooks
    QVector<Notebook> openNotebooks = {};
};

#endif // RESOURCEHANDLER_H
