/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef DRAGLAYOUT_H
#define DRAGLAYOUT_H

#include <QWidget>
#include "textbox.h"
#include "structure/page.h"
#include "constants.h"
#include "scalableqwidget.h"

/**
 * @brief DragLayout - This class handles user operations on DragLayouts, which are present in every page.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */
class DragLayout : public ScalableQWidget
{

public:
    explicit DragLayout(QWidget *parent = nullptr, Page *parentPage = nullptr, bool *queryUpdated = nullptr);
    TextBox *newTextBoxAtLocation(QString uuid, QPoint point, int width, QString content);
    Page *parentPage;
    const QObjectList &children() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    int getHeight(TextBox *tBox);

private:
    bool *queryUpdated;
};
#endif // DRAGLAYOUT_H
