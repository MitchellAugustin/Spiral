#ifndef DRAGLAYOUT_H
#define DRAGLAYOUT_H

#include <QWidget>
#include <textbox.h>

class DragLayout : public QWidget
{
public:
    explicit DragLayout(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};
#endif // DRAGLAYOUT_H
