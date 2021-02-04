/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef SPIRALCOMPONENT_H
#define SPIRALCOMPONENT_H

#include <QString>
#include <QVector>
#include <QUuid>
#include <QStringList>
#include <QDebug>

/**
 * @brief SpiralComponent - Base structure class for notebook component objects. Extended by Notebook, Section, and Page.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */
class SpiralComponent
{
public:
    SpiralComponent(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) {
        this->uuid = uuid;
        //If an empty UUID is read, generate a new one and assign it to the SpiralComponent.
        if (uuid.isEmpty()) {
            QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0];
            setUUID(uuid);
        }
    }
    const QString class_name = "SpiralComponent";
    /** For the time being, I am going to provide a UUID variable,
     * but since everything will be saved in one file, it may not be necessary.
     * */
    void setName(QString name) {
        this->name = name;
    }

    void setUUID(QString uuid) {
        this->uuid = uuid;
    }

    QString getName() {
        return this->name;
    }

    QString getUUID() {
        return this->uuid;
    }

private:
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
