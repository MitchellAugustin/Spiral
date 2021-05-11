#ifndef SCALABLEQWIDGET_H
#define SCALABLEQWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include <QWheelEvent>
#include <QDebug>

class ScalableQWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ScalableQWidget(QWidget *parent = nullptr);

protected Q_SLOTS:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    qreal scaleFactor;
signals:

};

#endif // SCALABLEQWIDGET_H
