#ifndef QZOOMSCROLLAREA_H
#define QZOOMSCROLLAREA_H

#include <QScrollArea>
#include <QWidget>
#include <QWheelEvent>

class QZoomScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    QZoomScrollArea(QWidget *parent);
};

#endif // QZOOMSCROLLAREA_H
