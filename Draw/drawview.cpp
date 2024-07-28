#include "drawview.h"
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>

const double MAX_SCALE = 5;
const double MIN_SCALE = 0.2;

DrawView::DrawView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setAcceptDrops(true);
    //    setOptimizationFlag(QGraphicsView::OptimizationFlag::DontAdjustForAntialiasing);
    //    setCacheMode(QGraphicsView::CacheModeFlag::CacheBackground);


    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
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

void DrawView::wheelEvent(QWheelEvent* event)
{
    // 获取当前的缩放比例
    qreal scaleFactor = transform().m11();

    // 计算新的缩放比例
    qreal newScaleFactor = scaleFactor;
    if (event->angleDelta().y() > 0)
    {
        // 向上滚动，放大
        newScaleFactor *= 1.25; // 可以调整这个值来控制放大的速度
        newScaleFactor = qMin(newScaleFactor, MAX_SCALE); // 确保不超过最大缩放比例
    }
    else
    {
        // 向下滚动，缩小
        newScaleFactor /= 1.25; // 可以调整这个值来控制缩小的速度
        newScaleFactor = qMax(newScaleFactor, MIN_SCALE); // 确保不小于最小缩放比例
    }

    // 应用新的缩放比例
    scale(newScaleFactor / scaleFactor, newScaleFactor / scaleFactor);
}
