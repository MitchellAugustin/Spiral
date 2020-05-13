#ifndef DRAGLAYOUT_H
#define DRAGLAYOUT_H

#include <QWidget>
#include <textbox.h>
#include "structure/page.h"
#include "constants.h"

class DragLayout : public QWidget
{
public:
    explicit DragLayout(QWidget *parent = nullptr, Page *parentPage = nullptr);
    void newTextBoxAtLocation(QPoint point);
    Page *parentPage;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
#endif // DRAGLAYOUT_H
