#ifndef DRAWVIEW_H
#define DRAWVIEW_H

#include <QGraphicsView>

class DrawView : public QGraphicsView
{
    Q_OBJECT
public:
    DrawView(QGraphicsScene* scene, QWidget* parent = nullptr);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    float         m_scale = 1; // 缩放比例

};

#endif // DRAWVIEW_H
