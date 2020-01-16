#ifndef DRAGRESIZELABEL_H
#define DRAGRESIZELABEL_H

#include <QLabel>
#include <QtWidgets>
#include "constants.h"

class DragResizeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DragResizeLabel(QWidget *parent = nullptr, bool expand = true);
    bool expand;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

};

#endif //DRAGRESIZELABEL_H
