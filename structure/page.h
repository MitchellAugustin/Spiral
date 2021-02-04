/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef PAGE_H
#define PAGE_H

#include <QVector>
#include <QJsonArray>

#include "spiralcomponent.h"
#include "textbox.h"

/**
 * @brief Page - Structure class for Page objects.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */
class Page : public SpiralComponent
{
public:
    Page(QString uuid = QUuid::createUuid().toString().split("{")[1].split("-")[0]) : SpiralComponent(uuid) {
        qDebug() << "Page instantiated with UUID " << uuid;
    }
    ~Page() {
        for(QVector<TextBox*>::Iterator t_it = textBoxList.begin(); t_it != textBoxList.end(); ++t_it) {
            delete *t_it;
            *t_it = nullptr;
        }
        delete dragLayout;
        dragLayout = nullptr;
        //Note: Do not delete editorPane here since deleting dragLayout also deletes it.
        editorPane = nullptr;
    }
    const QString class_name = "Page";

    bool opened = false;
    QJsonArray textboxes;
    QWidget *editorPane = nullptr;
    QWidget *dragLayout = nullptr;
    QVector<TextBox*> textBoxList;
};

#endif // PAGE_H
