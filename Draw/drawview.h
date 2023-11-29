#ifndef DRAWVIEW_H
#define DRAWVIEW_H

#include <QGraphicsView>

class DrawView : public QGraphicsView
{
    Q_OBJECT
public:
    DrawView(QGraphicsScene *scene,QWidget *parent=nullptr);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // DRAWVIEW_H
