#include "ellipseitem.h"
#include <QCursor>
#include <QDebug>

EllipseItem::EllipseItem(QPointF point,QGraphicsEllipseItem *parent)
    :QGraphicsEllipseItem (parent),
	ItemBase(this),
	m_starPos(point),
	m_color(Qt::blue),
	m_lineColor(Qt::black),
	m_lineSize(1),
	m_shapeSpace(0)
{
	m_lastAngle = m_angle;
	setFlag(ItemIsMovable, true);
	setFlag(ItemIsSelectable, true);
	setFlag(ItemSendsGeometryChanges, true);
	setAcceptHoverEvents(true);

	setPos(point);
}

EllipseItem::EllipseItem(const EllipseItem *item, QGraphicsEllipseItem *parent /*= nullptr*/)
	: QGraphicsEllipseItem(parent),
	  ItemBase(this)
{
	m_shapeSpace = 10;
	m_starPos.setX(item->m_starPos.x() + m_shapeSpace);
	m_starPos.setY(item->m_starPos.y() + m_shapeSpace);
	m_color = item->m_color;
	m_lineColor = item->m_lineColor;
	m_lineSize = item->m_lineSize;
	m_rect = item->m_rect;
	m_angle = item->m_angle;

	m_lastAngle = m_angle;

	setRotation(m_angle);
	setFlag(ItemIsMovable, true);
	setFlag(ItemIsSelectable, true);
	setFlag(ItemSendsGeometryChanges, true);
	setAcceptHoverEvents(true);
	setPos(m_starPos);
	setRect(m_rect);
	updateGeometry();
}

void EllipseItem::setEndPoint(const QPointF &point)
{
	m_endPos = point;

	m_rect.setWidth(point.x() - m_starPos.x());
	m_rect.setHeight(point.y() - m_starPos.y());

	setRect(m_rect);
}

//void EllipseItem::setStarPoint(const QPointF &point)
//{
//	m_starPos = point;
//}

void EllipseItem::setBrushColor(const QColor color)
{
	m_color = color;
}

void EllipseItem::setLineColor(QColor color)
{
	m_lineColor = color;
}

QColor EllipseItem::getLineColor() const
{
	return m_lineColor;
}

void EllipseItem::setLineSize(qreal size)
{
	m_lineSize = size;
}

qreal EllipseItem::getLineSize() const
{
	return m_lineSize;
}

void EllipseItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
	QPointF local = mapFromScene(point);
	QRect delta = m_rect.toRect();
	switch (dir) {
	case SizeHandleRect::LeftTop:
		delta.setTopLeft(local.toPoint());

		break;
	case SizeHandleRect::Top:
		delta.setTop(local.y());
		break;

	case SizeHandleRect::RightTop:
		delta.setTopRight(local.toPoint());
		break;

	case SizeHandleRect::Left:
		delta.setLeft(local.x());
		break;
	case SizeHandleRect::Right:
		delta.setRight(local.x());
		break;

	case SizeHandleRect::LeftBottom:
		delta.setBottomLeft(local.toPoint());
		break;

	case SizeHandleRect::Bottom:
		delta.setBottom(local.y());
		break;

	case SizeHandleRect::RightBottom:
		delta.setBottomRight(local.toPoint());
		break;

	default:
		break;
	}
	prepareGeometryChange();
	m_rect = delta;
	qDebug() << "ellipse item" << m_rect << endl;
	updateGeometry();
}

QGraphicsItem * EllipseItem::duplicate() const
{
	return new EllipseItem(this);
}

QColor EllipseItem::getBrushColor() const
{
	return m_color;
}

void EllipseItem::setRectInf(const QRectF &rect)
{
	m_rect = rect;
	setRect(m_rect);
}

void EllipseItem::setAngle(const qreal &angle)
{
	m_lastAngle = m_angle;
	m_angle = angle;
	setTransformOriginPoint(m_rect.x() + m_rect.width() / 2, m_rect.y() + m_rect.height() / 2);
	setRotation(angle);
}

void EllipseItem::saveFileToXml(QDomDocument &doc, QDomElement &root)
{
	QDomElement Ellipse = doc.createElement("Ellipse");
	QPointF startPoint=mapToScene(QPointF(m_rect.x(),m_rect.y()));
	Ellipse.setAttribute("angle",m_angle);
	Ellipse.setAttribute("x", startPoint.x());
	Ellipse.setAttribute("y", startPoint.y());
	Ellipse.setAttribute("width", m_rect.width());
	Ellipse.setAttribute("height", m_rect.height());
	root.appendChild(Ellipse);
	
	QDomElement pen = doc.createElement("Pen");
	pen.setAttribute("red", m_lineColor.red());
	pen.setAttribute("green", m_lineColor.green());
	pen.setAttribute("blue", m_lineColor.blue());
	pen.setAttribute("alpha", m_lineColor.alpha());
	pen.setAttribute("width", m_lineSize);
	Ellipse.appendChild(pen);

	QDomElement Brush = doc.createElement("Brush");
	Brush.setAttribute("red", m_color.red());
	Brush.setAttribute("green", m_color.green());
	Brush.setAttribute("blue", m_color.blue());
	Brush.setAttribute("alpha", m_color.alpha());
	Ellipse.appendChild(Brush);
}

void EllipseItem::loadFileFormXml(QDomElement &element)
{
	qreal angle = element.attribute("angle").toDouble();
	setAngle(angle);

	qreal x = element.attribute("x").toDouble();
	qreal y = element.attribute("y").toDouble();
	int w = element.attribute("width").toInt();
	int h = element.attribute("height").toInt();
	setRectInf(QRectF(0, 0, w, h));
	setPos(x, y);

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
			if (e.nodeName() == "Brush")
			{
				int r1 = e.attribute("red").toInt();
				int g1 = e.attribute("green").toInt();
				int b1 = e.attribute("blue").toInt();
				int alhpa1 = e.attribute("alpha").toInt();
				setBrushColor(QColor(r1, g1, b1, alhpa1));
			}
			n = n.nextSibling();
		}
	}
	updateGeometry();
}

void EllipseItem::saveFileToJson(QJsonArray &jsonArr)
{
	QJsonObject obj;
	obj.insert("name", "Ellipse");
	obj.insert("angle", m_angle);

	//setAngle(0);
	QPointF startPoint = mapToScene(QPointF(m_rect.x(), m_rect.y()));
	obj.insert("x", (int)startPoint.x());
	obj.insert("y", (int)startPoint.y());
	obj.insert("width", m_rect.width());
	obj.insert("height", m_rect.height());

	QJsonObject subObj1;
	subObj1.insert("red", m_lineColor.red());
	subObj1.insert("green", m_lineColor.green());
	subObj1.insert("blue", m_lineColor.blue());
	subObj1.insert("alhpa", m_lineColor.alpha());
	subObj1.insert("width", m_lineSize);
	obj.insert("Pen",subObj1);

	QJsonObject subObj2;
	subObj2.insert("red", m_color.red());
	subObj2.insert("green", m_color.green());
	subObj2.insert("blue", m_color.blue());
	subObj2.insert("alhpa", m_color.alpha());
	obj.insert("Brush", subObj2);

	jsonArr.append(obj);
}

void EllipseItem::loadFileFormJson(QJsonObject &jsonObj)
{
	qreal angle = jsonObj["angle"].toDouble();
	setAngle(angle);

	qreal x = jsonObj["x"].toDouble();
	qreal y = jsonObj["y"].toDouble();
	qreal w = jsonObj["width"].toDouble();
	qreal h = jsonObj["height"].toDouble();
	setRectInf(QRectF(0, 0, w, h));
	setPos(x, y);

	QJsonValue objValue = jsonObj.value("Pen");
	QJsonObject subObj = objValue.toObject();
	int r = subObj["red"].toInt();
	int g = subObj["green"].toInt();
	int b = subObj["blue"].toInt();
	int a = subObj["alhpa"].toInt();
	setLineColor(QColor(r, g, b, a));

	int penWidth = subObj["width"].toInt();
	setLineSize(penWidth);

	QJsonValue objValue2 = jsonObj.value("Brush");
	QJsonObject subObj2 = objValue2.toObject();
	r = subObj2["red"].toInt();
	g = subObj2["green"].toInt();
	b = subObj2["blue"].toInt();
	a = subObj2["alhpa"].toInt();
	setBrushColor(QColor(r, g, b, a));

	updateGeometry();
}

int EllipseItem::type() const
{
	return Type;
}

QVariant EllipseItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedChange) {
		emit itemsChange(this);
		setZValue(value.toBool() ? 1 : 0);
		setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
	}
	if (change == QGraphicsItem::ItemPositionChange) {
		checkSizeHandleRectContent();
	}
	if (change == QGraphicsItem::ItemPositionHasChanged)
	{
		updateLine();
	}
	return value;
}


void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);
	//抗锯齿
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	if (isSelected())
	{
		//设置轮廓为白色
		painter->setPen(QPen(Qt::black, 2, Qt::DotLine));
		painter->drawRect(m_rect);
	}
	painter->setPen(QPen(m_lineColor,m_lineSize));
	painter->setBrush(QBrush(m_color));
	qDebug() << "paint" << m_rect << "bounding rect"<< boundingRect() << endl;
    painter->drawEllipse(m_rect);
}

void EllipseItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	//setSelected(true);
	m_lastPos = pos();
	QGraphicsItem::mousePressEvent(event);
}

void EllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//setCursor(QCursor(Qt::SizeAllCursor));
	QGraphicsItem::mouseMoveEvent(event);
}

