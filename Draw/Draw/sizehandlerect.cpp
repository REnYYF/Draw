#include "sizehandlerect.h"
#include "rectitem.h"
#include "lineitem.h"
#include "drawscene.h"
#include <QPainter>
#include <QDebug>

SizeHandleRect::SizeHandleRect(QGraphicsItem *parent, QRectF rect, Direction dir) :
    QGraphicsRectItem(rect, parent),
    m_dir(dir),
    m_state(SelectionHandleOff),
	m_connectStatus(NotConnected)
{
    //setParentItem(parent);
    hide();
	m_rect = rect;
	m_listSizeHandle.clear();
	setRect(m_rect);
	setFlag(ItemIsMovable, true);
	//setFlag(ItemIsSelectable, true);
	setFlag(ItemSendsGeometryChanges, true);
	setAcceptHoverEvents(true);
}

SizeHandleRect::Direction SizeHandleRect::dir() const
{
    return m_dir;
}

bool SizeHandleRect::hitTest(const QPointF &point)
{
    QPointF pt = mapFromScene(point);
    return rect().contains(pt);
}

void SizeHandleRect::move(qreal x, qreal y)
{
    setPos(x,y);
}

void SizeHandleRect::setState(SelectionHandleState st)
{
    if (st == m_state)
        return;
    switch (st) {
    case SelectionHandleOff:
        hide();
        break;
    case SelectionHandleInactive:
    case SelectionHandleActive:
        show();
        break;
    }
    m_state = st;
}

int SizeHandleRect::type() const
{
	return Type;
}


void SizeHandleRect::setConnectStatus(ConnectState st)
{
	m_connectStatus = st;
}

int SizeHandleRect::getConnectStatus()
{
	return m_connectStatus;
}

void SizeHandleRect::adsorptionPointState(int distance)
{
	setRect(m_rect.adjusted(-distance, -distance, distance, distance));
	QList<QGraphicsItem *> temItemList = collidingItems();
	setRect(m_rect);
	for (auto varOldItem : collisionItem)
	{
		bool flag = false;
		for (auto varItem : temItemList)
		{
			if ((varItem->type() != SIZE_HANDLE_RECT) && (varOldItem->type() != SIZE_HANDLE_RECT))
			{
				if (varItem == varOldItem)
				{
					flag = true;
					break;
				}
			}
		}
		if (varOldItem->type() != SIZE_HANDLE_RECT)
		{
			if (!flag)
			{
				ItemBase *item = acquireItemType(varOldItem);
				item->setState(SelectionHandleOff);
			}
		}
	}
	collisionItem.clear();
	collisionItem = temItemList;
	for (auto item : collisionItem)
	{
		auto itemBase = acquireItemType(item);
		if (itemBase)
		{
			if (item->type() == ELLIPSE_ITEM_TYPE)
			{
				itemBase->m_handles[Top]->setState(SelectionHandleActive);
				itemBase->m_handles[Left]->setState(SelectionHandleActive);
				itemBase->m_handles[Right]->setState(SelectionHandleActive);
				itemBase->m_handles[Bottom]->setState(SelectionHandleActive);
			}
			else
			{
				itemBase->setState(SelectionHandleActive);
			}
		}
	}
}

QVariant SizeHandleRect::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange)
	{
		if ((data(ADSORBENT).toString()) == "adsorbent")
		{
			//显示、隐藏图元吸附点
			adsorptionPointState(50);
			for (auto item : collisionItem)
			{
				if (item->type() == SIZE_HANDLE_RECT)
				{
					setRect(m_rect.adjusted(-15, -15, 15, 15));
					QList<QGraphicsItem *> adsorbentList = collidingItems();
					setRect(m_rect);

					if (!adsorbentList.isEmpty())
					{
						QGraphicsItem *adsorbentItem = adsorbentList.first();
						if ((adsorbentItem->type()) == SIZE_HANDLE_RECT)
						{
							SizeHandleRect *itemSizeHandle = (SizeHandleRect *)adsorbentItem;
							QPointF newPos = itemSizeHandle->scenePos();
							auto p = parentItem()->mapToScene(value.toPointF());
							//大于15拖开，小于吸附
							if (qAbs(p.y() - newPos.y()) > 15 || qAbs(p.x() - newPos.x()) > 15)
							{
								return value;
							}
							emit adsorbentChange(this, QPointF(QPointF(4, 4) + newPos));
							bool flag = false;
							for (auto var : itemSizeHandle->m_listSizeHandle)
							{
								if (var == this)
								{
									flag = true;
								}
							}
							if (!flag && getConnectStatus() == NotConnected)
							{
								itemSizeHandle->m_listSizeHandle.append(this);
								setConnectStatus(Connected);
							}
							qDebug() << "this->scenePos() :" << this->scenePos() << "newPos :" << newPos << endl;
							newPos = parentItem()->mapFromScene(newPos);
							return newPos;
						}
					}
				}
			}
		}
	}
	if (change == QGraphicsItem::ItemPositionHasChanged)
	{
		if ((data(ADSORBENT).toString()) != "adsorbent")
		{
			for (auto var : this->m_listSizeHandle)
			{
				//刷新线条端点坐标，更新线条
				emit var->adsorbentChange(var, this->sceneBoundingRect().center());
				QPointF p = var->parentItem()->mapFromScene(this->scenePos());
				var->move(p.x(), p.y());
				qDebug() << "line total -> " << this->m_listSizeHandle.count() << endl;
			}
		}
	}
	qDebug() << "hasChange :" << this->scenePos() << endl;
	return QGraphicsItem::itemChange(change, value);
}


void SizeHandleRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mouseMoveEvent(event);
}

void SizeHandleRect::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	//m_stance = 50;

	QGraphicsRectItem::mousePressEvent(event);
}

void SizeHandleRect::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	//m_stance = 0;


	QGraphicsRectItem::mouseReleaseEvent(event);
}

void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//painter->fillRect(boundingRect(), Qt::black);
    QColor c = QColor("lime green");
  //  c.setAlpha(200);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(c));

    painter->drawRect(rect());
}

bool SizeHandleRect::collidesWithItem(const QGraphicsItem * other, Qt::ItemSelectionMode mode) const
{
	if (other == this || other->type() == LineItem::Type)
	{
		return false;
	}

	return QGraphicsItem::collidesWithItem(other, mode);
}

