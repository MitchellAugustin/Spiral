#ifndef SPIRALCOMPONENT_H
#define SPIRALCOMPONENT_H

#include <QString>
#include <QVector>
#include <QUuid>
#include <QStringList>
#include <QDebug>

class SpiralComponent
{
public:
    SpiralComponent(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) {
        this->uuid = uuid;
    }
    const QString class_name = "SpiralComponent";
    /** For the time being, I am going to provide a UUID variable,
     * but since everything will be saved in one file, it may not be necessary.
     * */
    QString uuid;
    QString name;
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
