/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef SECTION_H
#define SECTION_H

#include <QVector>
#include <QString>
#include <QStringList>

#include "spiralcomponent.h"
#include "page.h"

/**
 * @brief Section - Structure class for Section objects.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */
class Section : public SpiralComponent
{
public:
    Section(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) : SpiralComponent(uuid) {
        pagesList = new QVector<Page*>();
        qDebug() << "Section instantiated with UUID " << uuid;
    }
    ~Section() {
        for(QVector<Page*>::Iterator p_it = pagesList->begin(); p_it != pagesList->end(); ++p_it) {
            delete *p_it;
            *p_it = nullptr;
        }
        delete pagesList;
        pagesList = nullptr;
    }
    void addPage(Page *page) {
        pagesList->append(page);
        //Add page to UI
    }
    void removePage(Page *page) {
        pagesList->removeOne(page);
    }
    void removePage(int index) {
        pagesList->removeAt(index);
    }
    QVector<Page*> *loadPagesList() {
        return pagesList;
    }
    const QString class_name = "Section";

private:
    QVector<Page*> *pagesList = {};
};

#endif // SECTION_H
