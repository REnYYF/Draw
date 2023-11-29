#include "lineitem.h"
#include "rectitem.h"

#include <QPainter>
#include <QDebug>
#include <QCursor>

LineItem::LineItem(QPointF point,QGraphicsLineItem *parent)
    : QGraphicsLineItem (parent),
	  ItemBase(this),
	  m_endPoint(point),
	  m_starPoint(0,0),
	  m_lineColor(Qt::black),
	  m_lineSize(3),
	  m_shapeSpace(0)
{
	m_lastAngle = m_angle;
	Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
		delete (*it);
	m_handles.clear();

	for (int i = 1; i <= 2; ++i) 
	{
		SizeHandleRect *shr = new SizeHandleRect(this, QRectF(0, 0, 8, 8), static_cast<SizeHandleRect::Direction>(i));
		shr->setData(ADSORBENT, "adsorbent");
		//shr->setData(ADSORBENT, true);
		connect(shr, SIGNAL(adsorbentChange(SizeHandleRect *, QPointF &)), this, SLOT(adsorbent(SizeHandleRect *,QPointF &)));
		m_handles.push_back(shr);
	}
	
	updateGeometry();
	setFlag(ItemIsMovable, true);
	setFlag(ItemIsSelectable, true);
	setFlag(ItemSendsGeometryChanges, true);
	setAcceptHoverEvents(true);
	setPos(point);
}

LineItem::LineItem(const LineItem *item, QGraphicsLineItem *parent /*= nullptr*/)
	: QGraphicsLineItem(parent),
	  ItemBase(this)
{
	m_shapeSpace = 15;
	QLineF line=item->line();
	QPointF star = item->mapToScene(line.p1());
	QPointF end = item->mapToScene(line.p2());
	m_starPoint.setX(star.x() - m_shapeSpace);
	m_starPoint.setY(star.y() + m_shapeSpace);
	m_endPoint.setX(end.x() - m_shapeSpace);
	m_endPoint.setY(end.y() + m_shapeSpace);

	setPos(m_starPoint);
	m_endPoint = mapFromScene(m_endPoint);
	setLine(0, 0, m_endPoint.x(), m_endPoint.y());
	m_starPoint = mapFromScene(m_starPoint);

	m_lineColor = item->m_lineColor;
	m_lineSize = item->m_lineSize;
	m_angle = item->m_angle;
	setRotation(m_angle);
	Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
		delete (*it);
	m_handles.clear();

	m_lastAngle = m_angle;

	for (int i = 1; i <= 2; ++i)
	{
		SizeHandleRect *shr = new SizeHandleRect(this, QRectF(0, 0, 8, 8), static_cast<SizeHandleRect::Direction>(i));
		m_handles.push_back(shr);
	}
	updateGeometry();
	setFlag(ItemIsMovable, true);
	setFlag(ItemIsSelectable, true);
	setFlag(ItemSendsGeometryChanges, true);
	setAcceptHoverEvents(true);

}

LineItem::~LineItem()
{
}

void LineItem::setEndPoint(QPointF point)
{

	//m_endPoint=point;
	m_endPoint = mapFromScene(point);
	setLine(m_starPoint.x(), m_starPoint.y(), m_endPoint.x() , m_endPoint.y());
	//update();
}

void LineItem::setLineColor(QColor color)
{
	m_lineColor = color;
}

QColor LineItem::getLineColor() const
{
	return m_lineColor;
}

void LineItem::setLineSize(qreal size)
{
	m_lineSize = size;
}

qreal LineItem::getLineSize() const
{
	return m_lineSize;
}

void LineItem::updateGeometry()
{
	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
		SizeHandleRect *hndl = *it;;
		switch (hndl->dir()) {
		case SizeHandleRect::Top:
			hndl->move(m_starPoint.x() - selection_handle_size / 2, m_starPoint.y() - selection_handle_size / 2);
			break;
		case SizeHandleRect::RightTop:
			hndl->move(m_endPoint.x() - selection_handle_size / 2, m_endPoint.y() - selection_handle_size / 2);
			break;
		default:
			break;
		}
	}
}

void LineItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
	QPointF p = pos();

	switch (dir)
	{
	case SizeHandleRect::Top:
		m_starPoint = mapFromScene(point);
		break;
	case SizeHandleRect::RightTop:
		m_endPoint= mapFromScene(point);
		break;
	default:
		break;
	}
	prepareGeometryChange();
	updateGeometry();
	m_rect = QRectF(m_starPoint, m_endPoint);
	setLine(m_starPoint.x(), m_starPoint.y(), m_endPoint.x(), m_endPoint.y());
	update();

}

QGraphicsItem * LineItem::duplicate() const
{
	return new LineItem(this);
}

QRectF LineItem::getRect() const
{
	return QRectF(m_starPoint,m_endPoint);
}


void LineItem::setRectInf(const QRectF &rect)
{
	m_rect = rect;
	m_starPoint = QPointF(rect.x(), rect.y());
	//setPos(m_starPoint);
	m_endPoint = rect.bottomRight();
	setLine(m_starPoint.x(), m_starPoint.y(), m_endPoint.x(),m_endPoint.y());
}

void LineItem::saveFileToXml(QDomDocument &doc, QDomElement &root)
{
	QDomElement line = doc.createElement("Line");
	QPointF startPoint = mapToScene(m_starPoint);
	QPointF finishPoint = mapToScene(m_endPoint);
	line.setAttribute("sx", startPoint.x());
	line.setAttribute("sy", startPoint.y());
	line.setAttribute("fx", finishPoint.x());
	line.setAttribute("fy", finishPoint.y());
	root.appendChild(line);

	QDomElement pen = doc.createElement("Pen");
	pen.setAttribute("red", m_lineColor.red());
	pen.setAttribute("green", m_lineColor.green());
	pen.setAttribute("blue", m_lineColor.blue());
	pen.setAttribute("alpha", m_lineColor.alpha());
	pen.setAttribute("width", m_lineSize);
	line.appendChild(pen);
}

void LineItem::loadFileFormXml(QDomElement &element)
{
	qreal sx = element.attribute("sx").toDouble();
	qreal sy = element.attribute("sy").toDouble();
	qreal fx = element.attribute("fx").toDouble();
	qreal fy = element.attribute("fy").toDouble();

	setRectInf(QRectF(QPointF(sx, sy), QPointF(fx, fy)));

	QDomNode n = element.firstChild();
	while (!n.isNull())
	{
		if (n.isElement())
		{
			QDomElement e = n.toElement();
			if (e.nodeName() == "Pen")
			{
				int r = e.attribute("red").toInt();
				int g = e.attribute("green").toInt();
				int b = e.attribute("blue").toInt();
				int alhpa = e.attribute("alpha").toInt();
				setLineColor(QColor(r, g, b, alhpa));

				int penWidth = e.attribute("width").toInt();
				setLineSize(penWidth);
			}
			n = n.nextSibling();
		}
		updateGeometry();
	}
}

void LineItem::saveFileToJson(QJsonArray &jsonArr)
{
	QJsonObject obj;
	obj.insert("name", "Line");
	obj.insert("angle", m_angle);

	QPointF startPoint = mapToScene(m_starPoint);
	QPointF finishPoint = mapToScene(m_endPoint);
	obj.insert("sx", (int)startPoint.x());
	obj.insert("sy", (int)startPoint.y());
	obj.insert("fx", (int)finishPoint.x());
	obj.insert("fy", (int)finishPoint.y());

	QJsonObject subObj1;
	subObj1.insert("red", m_lineColor.red());
	subObj1.insert("green", m_lineColor.green());
	subObj1.insert("blue", m_lineColor.blue());
	subObj1.insert("alhpa", m_lineColor.alpha());
	subObj1.insert("width", m_lineSize);
	obj.insert("Pen", subObj1);

	jsonArr.append(obj);
}

void LineItem::loadFileFormJson(QJsonObject &jsonObj)
{
	qreal sx = jsonObj["sx"].toDouble();
	qreal sy = jsonObj["sy"].toDouble();
	qreal fx = jsonObj["fx"].toDouble();
	qreal fy = jsonObj["fy"].toDouble();
	setRectInf(QRectF(QPointF(sx, sy), QPointF(fx, fy)));

	QJsonValue objValue = jsonObj.value("Pen");
	QJsonObject subObj = objValue.toObject();
	int r = subObj["red"].toInt();
	int g = subObj["green"].toInt();
	int b = subObj["blue"].toInt();
	int a = subObj["alhpa"].toInt();
	setLineColor(QColor(r, g, b, a));

	int penWidth = subObj["width"].toInt();
	setLineSize(penWidth);

	updateGeometry();
}

QRectF LineItem::boundingRect() const
{
	return QGraphicsLineItem::boundingRect();
}

bool LineItem::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode /*= Qt::IntersectsItemShape*/) const
{

	if (other == this)
	{
		return false;
	}
	for (auto item : m_handles)
	{
		if (other == item)
		{
			return false;
		}
	}

	return QGraphicsItem::collidesWithItem(other, mode);
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= nullptr*/)
{
	//painter->fillRect(boundingRect(), Qt::red);
	//boundingRect();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	painter->setPen(QPen(m_lineColor,m_lineSize));
	painter->drawLine(line());
}

int LineItem::type() const
{
	return Type;
}

QVariant LineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedHasChanged) {
		emit itemsChange(this);
		qDebug() << " Item Selected : " << value.toString();

		setZValue(value.toBool() ? 1 : 0);
		setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
	}
	else if (change == QGraphicsItem::ItemRotationHasChanged) {
		//emit itemsChange(this);
		qDebug() << "Item Rotation Changed:" << value.toString();
	}
	else if (change == QGraphicsItem::ItemTransformOriginPointHasChanged) {
		qDebug() << "ItemTransformOriginPointHasChanged:" << value.toPointF();
	}
	else if (change == QGraphicsItem::ItemPositionChange)
	{
		//emit itemsChange(this);
		qDebug() << "ItemPositionChange" << value <<endl;
	}
	return value;
}

void LineItem::adsorbent(SizeHandleRect *sizeItem, QPointF &point)
{
	switch (sizeItem->dir())
	{
	case SizeHandleRect::Top:
		m_starPoint = mapFromScene(point);
		break;
	case SizeHandleRect::RightTop:
		m_endPoint = mapFromScene(point);
		break;
	default:
		break;
	}
	prepareGeometryChange();
	//updateGeometry();
	m_rect = QRectF(m_starPoint, m_endPoint);
	setLine(m_starPoint.x(), m_starPoint.y(), m_endPoint.x(), m_endPoint.y());
	update();
}

void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsLineItem::mouseReleaseEvent(event);
}

void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	/*setSelected(true);*/
	m_lastPos = pos();
	QGraphicsLineItem::mousePressEvent(event);
}

void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//setCursor(QCursor(Qt::SizeAllCursor));
	QGraphicsLineItem::mouseMoveEvent(event);
}
