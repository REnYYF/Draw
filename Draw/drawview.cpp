#include "drawview.h"
#include <QDebug>
#include <QMouseEvent>

DrawView::DrawView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setAcceptDrops(true);
    //this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

void DrawView::mousePressEvent(QMouseEvent* event)
{
    //qDebug() << " DrawView::mousePressEvent " << event->pos() <<endl;
    QGraphicsView::mousePressEvent(event);
}

void DrawView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
