#include "graphicsproperty.h"

#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <QVBoxLayout>
#include <QDebug>

GraphicsProperty::GraphicsProperty(QWidget *parent)
    : QWidget(parent)
{
	m_propertyEditor = new QtTreePropertyBrowser(this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->addWidget(m_propertyEditor);
	setLayout(layout);

	m_varManager = new QtVariantPropertyManager(m_propertyEditor);
	m_varFactory = new QtVariantEditorFactory(m_propertyEditor);
	connect(m_varManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
		this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
	m_propertyEditor->setFactoryForManager(m_varManager, m_varFactory);
}

void GraphicsProperty::slotValueChanged(QtProperty *property, const QVariant &var)
{
	//qDebug() << "slotValueChanged" << endl;
	ItemBase *item = acquireItemType(m_item);
	DrawScene *scene = (DrawScene *)m_item->scene();
	QRectF rect = item->getRect();
	QList<QGraphicsItem *> itemList = { m_item };
	if (!m_propertyToId.isEmpty() && item)
	{
		if (m_propertyToId[property] == "X")
		{
			if (m_item->type() == LINE_ITEM_TYPE)
			{
				QPointF point = QPointF(var.toDouble(), rect.y());
				item->setRectInf(QRectF(point, rect.size()));
				item->updateGeometry();
			}
			else
			{
				m_item->setX(var.toDouble());
			}
			emit valuesChange();
			emit itemMoved(itemList);
		}
		if (m_propertyToId[property] == "Y")
		{
			if (m_item->type() == LINE_ITEM_TYPE)
			{
				QPointF point = QPointF(rect.x(), var.toDouble());
				item->setRectInf(QRectF(point, rect.size()));
				item->updateGeometry();
			}
			else
			{
				m_item->setY(var.toDouble());
			}
			emit valuesChange();
			emit itemMoved(itemList);
		}
		if (m_propertyToId[property] == "Pen")
		{
			//item->setPen(var.value<QColor>());
			emit itemLineColor(scene, var.value<QColor>());
			emit valuesChange();
			//qDebug() << var << endl;
		}
		if (m_propertyToId[property] == "Brush")
		{
			//item->setBrushColor(var.value<QColor>());
			emit itemBrushColor(scene, var.value<QColor>());
			emit valuesChange();
			//qDebug() << var << endl;
		}
		if (m_propertyToId[property] == "Size")
		{
			rect.setSize(var.value<QSize>());
			item->setRectInf(rect);
			item->updateGeometry();
			emit valuesChange();
			emit itemResize(m_item,rect);
			//qDebug() << var << endl;
		}
		if (m_propertyToId[property] == "Angle")
		{
			item->setAngle(var.toDouble());
			emit itemRotate(m_item,item->m_lastAngle);
			emit valuesChange();
			//qDebug() << var << endl;
		}
		if (m_propertyToId[property] == "Pen Width")
		{
			int w = var.toString().toInt();
			emit itemLineSize(scene, w + 1);
			emit valuesChange();
			//qDebug() << w << endl;
		}
	}
}

void GraphicsProperty::onItemsChange(QGraphicsItem *item)
{
	//qDebug() << "Property  onItemClicked" << item << endl;
	m_item = item;
	ItemBase *itemBase = acquireItemType(item);
	updateProperty();
	if ( (!item) || (!itemBase))
	{
		return;
	}
	QRectF rect = itemBase->getRect();
	QPointF point = item->mapToScene(rect.x(),rect.y());
	positionProperty(point);
	switch (item->type())
	{
	case ELLIPSE_ITEM_TYPE:
		penProperty(itemBase);
		angleProperty(itemBase);
		brushProperty(itemBase);
		sizeProperty(itemBase);
		break;
	case RECT_ITEM_TYPE:
		penProperty(itemBase);
		angleProperty(itemBase);
		brushProperty(itemBase);
		sizeProperty(itemBase);
		break;
	case LINE_ITEM_TYPE:
		penProperty(itemBase);
		break;
	case SVG_ITEM_TYPE:
		angleProperty(itemBase);
		brushProperty(itemBase);
		sizeProperty(itemBase);
		break;
	default:
		qDebug() << "no shape" << endl;
		break;
	}
}

void GraphicsProperty::positionProperty(const QPointF &point)
{
	QtVariantProperty *property = m_varManager->addProperty(QVariant::Double, tr("Position X"));
	property->setValue(point.x());
	m_idToProperty["X"] = property;
	m_propertyToId[property] = "X";
	m_propertyEditor->addProperty(property);
	
	property = m_varManager->addProperty(QVariant::Double, tr("Position Y"));
	property->setValue(point.y());
	m_idToProperty["Y"] = property;
	m_propertyToId[property] = "Y";
	m_propertyEditor->addProperty(property);
}

void GraphicsProperty::penProperty(ItemBase *item)
{
	QColor penColor = item->getLineColor();
	qreal penSize = item->getLineSize();

	QtVariantProperty *property = m_varManager->addProperty(QVariant::Color, tr("Pen Color"));
	property->setValue(penColor);
	m_idToProperty["Pen"] = property;
	m_propertyToId[property] = "Pen";

	QtVariantProperty *subProperty = m_varManager->addProperty(QtVariantPropertyManager::enumTypeId(), QStringLiteral("width"));
	QStringList enumNames;
	for (int i = 1; i <= 10; ++i)
	{
		enumNames << tr("%1px").arg(i);
	}
	subProperty->setAttribute(QLatin1String("enumNames"), enumNames);
	subProperty->setValue(penSize - 1);
	property->addSubProperty(subProperty);
	m_idToProperty["Pen Width"] = subProperty;
	m_propertyToId[subProperty] = "Pen Width";
	m_propertyEditor->addProperty(property);
}

void GraphicsProperty::brushProperty(ItemBase *item) 
{
	QColor brushColor = item->getBrushColor();
	QtVariantProperty *property = m_varManager->addProperty(QVariant::Color, tr("Brush Color"));
	property->setValue(brushColor);
	m_idToProperty["Brush"] = property;
	m_propertyToId[property] = "Brush";
	m_propertyEditor->addProperty(property);
}

void GraphicsProperty::sizeProperty(ItemBase *item)
{
	QRectF rect = item->getRect();
	QtVariantProperty *property = m_varManager->addProperty(QVariant::Size, tr("Size"));
	property->setValue(rect.size());
	m_idToProperty["Size"] = property;
	m_propertyToId[property] = "Size";
	m_propertyEditor->addProperty(property);
}

void GraphicsProperty::angleProperty(ItemBase *item)
{
	qreal angle = item->getAngle();
	QtVariantProperty *property = m_varManager->addProperty(QVariant::Double, tr("Angle"));
	property->setValue(angle);
	m_idToProperty["Angle"] = property;
	m_propertyToId[property] = "Angle";
	m_propertyEditor->addProperty(property);
}

void GraphicsProperty::updateProperty()
{
	if (m_idToProperty.isEmpty())
	{
		qDebug() << "m_property is empty" << endl;
		return;
	}

	QMap<QString, QtVariantProperty *>::ConstIterator itProp = m_idToProperty.constBegin();
	while (itProp != m_idToProperty.constEnd()) {
		delete itProp.value();
		itProp++;
	}
	m_idToProperty.clear();
	m_propertyToId.clear();
}
