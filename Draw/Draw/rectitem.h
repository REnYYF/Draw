#ifndef RECTITEM_H
#define RECTITEM_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QObject>
#include "itembase.h"

#define  RECT_ITEM_TYPE UserType + 4

class RectItem : public QObject, public QGraphicsRectItem , public ItemBase
{
	Q_OBJECT
public:
    RectItem(QPointF point, QGraphicsItem* parent = nullptr);
	RectItem(const RectItem *item, QGraphicsItem* parent = nullptr);
    ~RectItem();
    void setStartPoint(QPointF point);
	void setEndPoint(QPointF point);
	//更新矩形
	void resizeTo(SizeHandleRect::Direction dir, const QPointF & point) override;
	QRectF  rect() const;
	virtual QGraphicsItem * duplicate() const override;
	virtual void setLineSize(qreal size) override;
	virtual qreal getLineSize() const override;
	virtual void setLineColor(QColor color) override;
	virtual QColor getLineColor() const override;
	virtual void setBrushColor(const QColor color) override;
	virtual QColor getBrushColor() const override;
	virtual void setAngle(const qreal &angle) override;
	virtual void setRectInf(const QRectF &rect)override;
	virtual void saveFileToXml(QDomDocument &doc, QDomElement &root) override;
	virtual void loadFileFormXml(QDomElement &element) override;
	virtual void saveFileToJson(QJsonArray &jsonArr) override;
	virtual void loadFileFormJson(QJsonObject &jsonObj) override;

signals:
	void itemsChange(QGraphicsItem *item);

	// QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	virtual int type() const override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	virtual QRectF boundingRect() const override;

private:
    QPointF								m_starPos;
    QPointF								m_endPos;
	QColor								m_color;
	QColor								m_lineColor;
	qreal								m_lineSize;
	int									m_shapeSpace;
	enum { Type = RECT_ITEM_TYPE };


};

#endif // RECTITEM_H
