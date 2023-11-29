#ifndef ITEMBASE_H
#define ITEMBASE_H


#include <QPen>
#include <QDomDocument> 
#include <QDomElement>
#include <QJsonObject> 
#include <QJsonArray>
#include <QJsonDocument>
#include "sizehandlerect.h"

class ItemBase
{
public:
    ItemBase(QGraphicsItem *p1);
    ~ItemBase();
	//旋转
	void rotateTo(QGraphicsItem *item, const QPointF &endPoint);
	//判断鼠标是否落在SizeHandleRect
	virtual SizeHandleRect::Direction  hitTest(const QPointF & point) const;
	//拉伸
	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF &point);
	//拷贝
	virtual QGraphicsItem * duplicate() const { return NULL; }
	//更新SizeHandleRect
	virtual void updateGeometry();

	virtual void setState(SelectionHandleState st);

	virtual QRectF  rectBox() const;

	virtual void setAngle(const qreal &angle) { Q_UNUSED(angle); }
	virtual qreal getAngle() const { return m_angle; }

	virtual void setLineSize(qreal size) { Q_UNUSED(size); }
	virtual qreal getLineSize() const { return 1; }

	virtual void setLineColor(QColor color) { Q_UNUSED(color); }
	virtual QColor getLineColor() const { return nullptr; };

	virtual void setRectInf(const QRectF &rect) { Q_UNUSED(rect); }
	virtual QRectF getRect() const { return m_rect; }

	virtual void setBrushColor(const QColor burshColor) { Q_UNUSED(burshColor); }
	virtual QColor getBrushColor() const { return nullptr; }

	virtual void loadFileFormXml(QDomElement &element) { Q_UNUSED(element); }
	virtual void saveFileToXml(QDomDocument &doc, QDomElement &root) { Q_UNUSED(doc); Q_UNUSED(root); }

	virtual void loadFileFormJson(QJsonObject &jsonObj) { Q_UNUSED(jsonObj); }
	virtual void saveFileToJson(QJsonArray &jsonArr) { Q_UNUSED(jsonArr); }

	//检查图元移动之后，线条是否随之更新，将需要更新的线条端点保存，反之删除
	void checkSizeHandleRectContent();
	void updateLine();


public:
	typedef QVector<SizeHandleRect*> Handles;
	Handles		m_handles;
	int			selection_handle_size = 8; 
	QRectF		m_rect;
	qreal		m_angle;
	qreal		m_lastAngle;
	QPointF		m_lastPos;
};

#endif // ITEMBASE_H
