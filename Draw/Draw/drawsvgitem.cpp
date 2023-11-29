#include "drawsvgitem.h"
#include <QDebug>
#include <QCursor>


DrawSVGItem::DrawSVGItem(const DrawSVGItem * item, QGraphicsItem *parent)
    : QGraphicsSvgItem (parent),
	  ItemBase(this)
{
	m_shapeSpace = 10;
	m_point.setX(item->m_point.x() + m_shapeSpace);
	m_point.setY(item->m_point.y() + m_shapeSpace);
	m_svgFile = item->m_svgFile;
	m_angle = item->m_angle;
	m_rect = item->m_rect;
	setRotation(m_angle);
	QFile file(m_svgFile);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug() << "DrawSVGItem::DrawSVGItem(const QString &svgFile,QGraphicsItem *parent): file open failed";
		return;
	}

	m_svgXML.setContent(file.readAll());
	changeAttributes("fill", item->m_color.name().toUpper());
	setSharedRenderer(new QSvgRenderer(m_svgXML.toByteArray()));
	initRect();

	m_lastAngle = m_angle;

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	this->setAcceptHoverEvents(true);
	setPos(m_point);
}

DrawSVGItem::DrawSVGItem(QPointF point,const QString &svgFile,QGraphicsItem *parent)
    : QGraphicsSvgItem (parent),
	  ItemBase(this),
      m_svgFile(svgFile),
      m_point(point),
	  m_color(Qt::black),
	  m_shapeSpace(0)
{

	m_rect.setRect(0,0,100,100);
    QFile file(svgFile);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "DrawSVGItem::DrawSVGItem(const QString &svgFile,QGraphicsItem *parent): file open failed";
        return;
    }

    m_svgXML.setContent(file.readAll());
    setSharedRenderer(new QSvgRenderer(m_svgXML.toByteArray()));
	initRect();

	m_lastAngle = m_angle;

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	this->setAcceptHoverEvents(true);
	setPos(point);
}

DrawSVGItem::~DrawSVGItem()
{

}


void DrawSVGItem::initRect()
{
	QRectF rect = boundingRect();
	//qDebug() << "boundingRect :" << rect << endl;
	m_rect.setRect(rect.x(),rect.y(),rect.width(),rect.height());
	updateGeometry();
}

void DrawSVGItem::resizeRect(const QRectF rect)
{
	//qDebug() << "rect width:" << rect.width() << endl;
    changeAttributes("width" , rect.width());
    changeAttributes("height" , rect.height());
	QDomElement rootElem = m_svgXML.documentElement();
	//qDebug() << "rect 222:" << m_rect << endl;
	setSharedRenderer(new QSvgRenderer(m_svgXML.toByteArray()));
	setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(rect.width(), rect.height()));
    update();
}

void DrawSVGItem::setBrushColor(const QColor color)
{
	m_color = color;
    changeAttributes("fill", color.name().toUpper());
    renderer()->load(m_svgXML.toByteArray());
	update();
}

QColor DrawSVGItem::getBrushColor() const
{
	return m_color;
}

void DrawSVGItem::setPoint(const QPointF &point)
{
    m_point=point;
}

void DrawSVGItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
	qDebug() << "point pos:" << point << endl;
	QPointF local = mapFromScene(point);

	QRect delta = m_rect.toRect();
	QPointF bottomR = delta.bottomRight();
	QPointF topL = delta.topLeft();

	switch (dir) {
	case SizeHandleRect::LeftTop:
		if (local.x() > bottomR.x())
		{
			local.setX(bottomR.x());
		}
		if (local.y() > bottomR.y())
		{
			local.setY(bottomR.y());
		}
		delta.setTopLeft(local.toPoint());
		
		break;
	case SizeHandleRect::Top:
		if (local.y() < bottomR.y())
		{
			delta.setTop(local.y());
		}
		else
		{
			delta.setTop(bottomR.y());
		}
		break;

	case SizeHandleRect::RightTop:
		if (local.x() < topL.x())
		{
			local.setX(topL.x());
		}
		if (local.y() > bottomR.y())
		{
			local.setY(bottomR.y());
		}
		delta.setTopRight(local.toPoint());
		break;

	case SizeHandleRect::Left:
		if (local.x() < bottomR.x())
		{
			delta.setLeft(local.x());
		}
		else
		{
			delta.setLeft(bottomR.x());
		}
		break;
	case SizeHandleRect::Right:
		if (local.x() > topL.x())
		{
			delta.setRight(local.x());
		}
		else
		{
			delta.setRight(topL.x());
		}
		break;

	case SizeHandleRect::LeftBottom:
		if (local.x() > bottomR.x())
		{
			local.setX(bottomR.x());
		}
		if (local.y() < topL.y())
		{
			local.setY(topL.y());
		}
		delta.setBottomLeft(local.toPoint());
		break;

	case SizeHandleRect::Bottom:
		if (local.y() > topL.y())
		{
			delta.setBottom(local.y());
		}
		else
		{
			delta.setBottom(topL.y());
		}
		break;

	case SizeHandleRect::RightBottom:
		if (local.x() < topL.x())
		{
			local.setX(topL.x());
		}
		if (local.y() < topL.y())
		{
			local.setY(topL.y());
		}
		delta.setBottomRight(local.toPoint());
		break;

	default:
		break;
	}
	prepareGeometryChange();
	m_rect = delta;
	resizeRect(delta);
	updateGeometry();
}

void DrawSVGItem::setRectInf(const QRectF &rect)
{
	m_rect = rect;
	resizeRect(rect);
}

QGraphicsItem * DrawSVGItem::duplicate() const
{
	return new DrawSVGItem(this);
}

void DrawSVGItem::setAngle(const qreal &angle)
{
	m_lastAngle = m_angle;
	m_angle = angle;
	setTransformOriginPoint(m_rect.x() + m_rect.width() / 2, m_rect.y() + m_rect.height() / 2);
	setRotation(angle);
}

void DrawSVGItem::saveFileToXml(QDomDocument &doc, QDomElement &root)
{
	QDomElement svg = doc.createElement("SvgGraphics");
	QPointF startPoint = scenePos();
	svg.setAttribute("angle", m_angle);
	svg.setAttribute("x", startPoint.x());
	svg.setAttribute("y", startPoint.y());
	svg.setAttribute("width", m_rect.width());
	svg.setAttribute("height", m_rect.height());
	svg.setAttribute("path", m_svgFile);
	root.appendChild(svg);

	QDomElement Brush = doc.createElement("Brush");
	Brush.setAttribute("red", m_color.red());
	Brush.setAttribute("green", m_color.green());
	Brush.setAttribute("blue", m_color.blue());
	Brush.setAttribute("alpha", m_color.alpha());
	svg.appendChild(Brush);
}

void DrawSVGItem::loadFileFormXml(QDomElement &element)
{
	qreal x = element.attribute("x").toDouble();
	qreal y = element.attribute("y").toDouble();
	int w = element.attribute("width").toInt();
	int h = element.attribute("height").toInt();
	setRectInf(QRectF(0, 0, w, h));
	setPos(x, y);

	qreal angle = element.attribute("angle").toDouble();
	m_lastAngle = m_angle;
	m_angle = angle;
	setTransformOriginPoint(0, 0);
	setRotation(angle);

	QDomNode n = element.firstChild();
	while (!n.isNull())
	{
		if (n.isElement())
		{
			QDomElement e = n.toElement();
			if (e.nodeName() == "Brush")
			{
				int r = e.attribute("red").toInt();
				int g = e.attribute("green").toInt();
				int b = e.attribute("blue").toInt();
				int alhpa = e.attribute("alpha").toInt();
				setBrushColor(QColor(r, g, b, alhpa));
							}
			n = n.nextSibling();
		}
		updateGeometry();
	}
}

void DrawSVGItem::saveFileToJson(QJsonArray &jsonArr)
{
	QJsonObject obj;
	obj.insert("name", "SvgGraphics");
	obj.insert("angle", m_angle);

	QPointF startPoint = mapToScene(QPointF(m_rect.x(), m_rect.y()));
	obj.insert("x", (int)startPoint.x());
	obj.insert("y", (int)startPoint.y());
	obj.insert("width", m_rect.width());
	obj.insert("height", m_rect.height());
	obj.insert("path", m_svgFile);

	QJsonObject subObj2;
	subObj2.insert("red", m_color.red());
	subObj2.insert("green", m_color.green());
	subObj2.insert("blue", m_color.blue());
	subObj2.insert("alhpa", m_color.alpha());
	obj.insert("Brush", subObj2);

	jsonArr.append(obj);
}

void DrawSVGItem::loadFileFormJson(QJsonObject &jsonObj)
{
	qreal angle = jsonObj["angle"].toDouble();
	setAngle(angle);

	qreal x = jsonObj["x"].toDouble();
	qreal y = jsonObj["y"].toDouble();
	qreal w = jsonObj["width"].toDouble();
	qreal h = jsonObj["height"].toDouble();
	setRectInf(QRectF(0, 0, w, h));
	setPos(x, y);

	QJsonValue objValue = jsonObj.value("Brush");
	QJsonObject subObj = objValue.toObject();
	int r = subObj["red"].toInt();
	int g = subObj["green"].toInt();
	int b = subObj["blue"].toInt();
	int a = subObj["alhpa"].toInt();
	setBrushColor(QColor(r, g, b, a));

	updateGeometry();
}

QRectF DrawSVGItem::boundingRect() const
{
	return m_rect;
}

int DrawSVGItem::type() const
{
	return Type;
}

void DrawSVGItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//setCursor(QCursor(Qt::SizeAllCursor));
	QGraphicsItem::mouseMoveEvent(event);
}

QVariant DrawSVGItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedHasChanged) {
		//qDebug() << " Item Selected : " << value.toString();
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

void DrawSVGItem::changeAttributes(const QString &attName, const QString &attValue)
{
    QDomElement rootElem = m_svgXML.documentElement();
	if (rootElem.hasAttribute(attName))
	{
		rootElem.setAttribute(attName, attValue);
		return;
	}
    QDomNode n = rootElem.firstChild();
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
			if (e.hasAttribute(attName))
			{
				e.setAttribute(attName, attValue);
			}
			if (n.hasChildNodes())
			{
				recursiveChangeAttributes(n.firstChild(), attName, attValue);
			}
        }
        n = n.nextSibling();
    }
}

void DrawSVGItem::changeAttributes(const QString &attName, const int size)
{
	//qDebug() << attName << "changeAttributes:" << size << endl;
    QDomElement rootElem = m_svgXML.documentElement();
    if (rootElem.hasAttribute(attName))
	{
		//qDebug() << attName << " : " << rootElem.attribute(attName) << endl;
        rootElem.setAttribute(attName, size);
		//qDebug() << "setAttribute(attName, size):" << rootElem.attribute(attName) << endl;
        return;
    }
}

void DrawSVGItem::recursiveChangeAttributes(const QDomNode &node, const QString &attName, const QString &attValue)
{
    QDomNode n = node;
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            if(e.hasAttribute(attName))
                e.setAttribute(attName, attValue);

            if(n.hasChildNodes())
                recursiveChangeAttributes(n.firstChild(), attName, attValue);
        }
        n = n.nextSibling();
    }
}
int num=0;
void DrawSVGItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   //qDebug()<<"click"<<num++<<endl;
	Q_UNUSED(widget);
	Q_UNUSED(option);
   if (isSelected())
    {
        //设置轮廓为白色
        painter->setPen(QPen(Qt::black,2,Qt::DotLine));
		//painter->setBrush(QBrush(Qt::red));
        painter->drawRect(m_rect);
    }
    renderer()->render(painter,m_rect);
	//QPointF p = mapToScene(m_rect.topLeft());
	//setPos(p);
	qDebug() << "svgpaint m_rect:" << m_rect <<"bounding rect:"<< boundingRect()<< endl;
	//qDebug() << "SvgPaint:" << m_rect << boundingRect() << endl;
}

void DrawSVGItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_lastPos = pos();
    QGraphicsItem::mousePressEvent(event);
}



