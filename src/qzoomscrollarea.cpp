#include "qzoomscrollarea.h"

QZoomScrollArea::QZoomScrollArea(QWidget *parent) : QScrollArea(parent)
{

}

void wheelEvent(QWheelEvent* e) {
    double OldScale = 1; // Get old scale factor
    double NewScale = 5; // Set new scale, use QWheelEvent...


    QPointF ScrollbarPos = QPointF(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QPointF DeltaToPos = e->posF() / OldScale - widget()->pos() / OldScale;
    QPointF Delta = DeltaToPos * NewScale - DeltaToPos * OldScale;

    widget()->resize(/* Resize according to new scale factor */);

    horizontalScrollBar()->setValue(ScrollbarPos.x() + Delta.x());
    verticalScrollBar()->setValue(ScrollbarPos.y() + Delta.y());
}
