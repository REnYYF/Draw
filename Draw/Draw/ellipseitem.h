#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QVariant>
#include "itembase.h"


#define  ELLIPSE_ITEM_TYPE UserType + 2 

class EllipseItem : public QObject, public QGraphicsEllipseItem , public ItemBase 
{
	Q_OBJECT
public:
    EllipseItem(QPointF point,QGraphicsEllipseItem *parent=nullptr);
	EllipseItem(const EllipseItem *item, QGraphicsEllipseItem *parent = nullptr);
	void setBrushColor(const QColor color);
	void resizeTo(SizeHandleRect::Direction dir, const QPointF &point) override;
	virtual QGraphicsItem * duplicate() const override;
	virtual QColor getBrushColor() const override;
	void setEndPoint(const QPointF &point);
	virtual void setLineSize(qreal size) override;
	virtual qreal getLineSize() const override;
	virtual void setLineColor(QColor color) override;
	virtual QColor getLineColor() const override;
	virtual void setRectInf(const QRectF &rect) override;
	virtual void setAngle(const qreal &angle) override;
	virtual void saveFileToXml(QDomDocument &docroot, QDomElement &root) override;
	virtual void loadFileFormXml(QDomElement &element) override;
	virtual void saveFileToJson(QJsonArray &jsonArr) override;
	virtual void loadFileFormJson(QJsonObject &jsonObj) override;

	// QGraphicsItem interface
public:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual int type() const override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

signals:
	void itemsChange(QGraphicsItem *item);

private:
	QPointF		m_starPos;
	QPointF		m_endPos;
	QColor		m_color;
	QColor		m_lineColor;
	qreal		m_lineSize;
	int			m_shapeSpace;
	enum { Type = ELLIPSE_ITEM_TYPE };

};

#endif // ELLIPSEITEM_H
