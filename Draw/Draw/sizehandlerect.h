#ifndef SIZEHANDLERECT_H
#define SIZEHANDLERECT_H

#include <QGraphicsRectItem>

#define UserType 65535
#define SIZE_HANDLE_RECT (UserType + 10)
#define ADSORBENT  0xFFFF

enum SelectionHandleState { SelectionHandleOff, SelectionHandleInactive, SelectionHandleActive };
enum ConnectState { Connected, NotConnected };

class SizeHandleRect : public QObject, public QGraphicsRectItem
{
Q_OBJECT
public:
	enum { Type = SIZE_HANDLE_RECT };

    enum Direction { LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left , Center, None};
    SizeHandleRect(QGraphicsItem* parent , QRectF rect, Direction dir);
    Direction dir() const;
    bool hitTest( const QPointF & point );
    void move(qreal x, qreal y );
    void setState(SelectionHandleState st);
	virtual int type() const override;

	//设置连接状态
	void setConnectStatus(ConnectState st);
	int getConnectStatus();

	/* 线条端点靠近吸附点，显示图元所有吸附点：
	** 线条端点远离吸附点，隐藏图元所有吸附点。*/
	void adsorptionPointState(int distance);
signals:
	void adsorbentChange(SizeHandleRect *item, QPointF &point);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	virtual bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const override;
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;


	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
	QList<SizeHandleRect *> m_listSizeHandle;//存放连线线条端点

private:
    const Direction			m_dir;
    SelectionHandleState	m_state;

	QList<QGraphicsItem *>  collisionItem;
	ConnectState			m_connectStatus;
	QRectF					m_rect;
};

#endif // SIZEHANDLERECT_H
