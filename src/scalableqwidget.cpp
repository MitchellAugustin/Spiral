#include "scalableqwidget.h"

ScalableQWidget::ScalableQWidget(QWidget *parent) : QGraphicsView(parent)
{
    //setDragMode(QGraphicsView::ScrollHandDrag);


}

void ScalableQWidget::paintEvent(QPaintEvent *event) {
    //scale(scaleFactor, scaleFactor);
    qDebug() << "Repainting/scaling";
}

void ScalableQWidget::wheelEvent(QWheelEvent *event) {
    scaleFactor += (event->delta()/120);
    qDebug() << "Scaling: " << scaleFactor;
    if (event->delta() > 0) {
        scale(1.25, 1.25);
    }
    else {
        scale(0.8, 0.8);
    }
}
