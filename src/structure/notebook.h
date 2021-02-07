/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include "spiralcomponent.h"
#include "section.h"

/**
 * @brief Notebook - Structure class for Notebook objects.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */
class Notebook : public SpiralComponent
{
public:
    Notebook(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) : SpiralComponent(uuid) {
        sectionsList = new QVector<Section*>();
        qDebug() << "Notebook instantiated with UUID " << uuid;
    }
    ~Notebook() {
        for(QVector<Section*>::Iterator s_it = sectionsList->begin(); s_it != sectionsList->end(); ++s_it) {
            delete *s_it;
            *s_it = nullptr;
        }
        delete sectionsList;
        sectionsList = nullptr;
    }
    void addSection(Section *section) {
        sectionsList->append(section);
        //Add section to UI
    }
    void removeSection(Section *section) {
        sectionsList->removeOne(section);
    }
    void removeSection(int index) {
        sectionsList->removeAt(index);
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
