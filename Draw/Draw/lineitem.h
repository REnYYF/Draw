#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>
#include "itembase.h"

//#define UserType 65536
#define LINE_ITEM_TYPE UserType + 3

class LineItem : public QObject, public QGraphicsLineItem , public ItemBase
{
	Q_OBJECT
public:
	enum { Type = LINE_ITEM_TYPE };

    LineItem(QPointF point,QGraphicsLineItem *parent=nullptr);
	LineItem(const LineItem *item, QGraphicsLineItem *parent = nullptr);
	~LineItem();
	void setEndPoint(QPointF Point);
	virtual void setLineColor(QColor color) override;
	virtual QColor getLineColor() const override;
	virtual void updateGeometry() override;
	virtual void setLineSize(qreal size) override;
	virtual qreal getLineSize() const override;
	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF &point);
	virtual QGraphicsItem * duplicate() const override;
	virtual QRectF getRect() const override;
	virtual void setRectInf(const QRectF &rect) override;
	virtual void saveFileToXml(QDomDocument &doc, QDomElement &root) override;
	virtual void loadFileFormXml(QDomElement &element) override;
	virtual void saveFileToJson(QJsonArray &jsonArr) override;
	virtual void loadFileFormJson(QJsonObject &jsonObj) override;


	QRectF boundingRect() const override;


signals:
	void itemsChange(QGraphicsItem *item);

public slots:
	void adsorbent(SizeHandleRect *sizeItem, QPointF &point);

protected:
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
	virtual int type() const override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	virtual bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const override;

private:
	QPointF		m_starPoint;
	QPointF		m_endPoint;
	QColor		m_lineColor;
	qreal		m_lineSize;
	int			m_shapeSpace;

	int         m_stance = 0;
};

#endif // LINEITEM_H
