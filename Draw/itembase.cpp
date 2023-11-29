#include "itembase.h"
#include <QGraphicsItem>
#include <QtMath>
#include <QDebug>

ItemBase::ItemBase(QGraphicsItem *p1)
	: m_angle(0)
{
	m_handles.reserve(SizeHandleRect::None);
	for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Left; ++i) {
		SizeHandleRect *shr = new SizeHandleRect(p1, QRectF(0, 0, 8, 8), static_cast<SizeHandleRect::Direction>(i));
		m_handles.push_back(shr);
	}
}

ItemBase::~ItemBase()
{

}

void ItemBase::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{

}

void ItemBase::rotateTo(QGraphicsItem *item, const QPointF &endPoint)
{
	QPointF centerPoint = item->mapToScene(QPointF(m_rect.center()));
	m_angle = atan2((endPoint.y() - centerPoint.y()), (endPoint.x() - centerPoint.x())) * 180 / M_PI;
	qDebug() << "rotateTo" << m_angle << endPoint<< centerPoint<< m_rect << item->mapToScene(QPointF(m_rect.x(),m_rect.y())) <<endl;
	m_angle = int(m_angle - m_lastAngle);

	if (m_angle > 360)
		m_angle -= 360;
	if (m_angle < -360)
		m_angle += 360;
	item->setTransformOriginPoint(m_rect.x() + m_rect.width() / 2, m_rect.y() + m_rect.height() / 2);
	item->setRotation(m_angle);
}


SizeHandleRect::Direction ItemBase::hitTest(const QPointF & point) const
{
	const Handles::const_iterator hend = m_handles.end();
	for (Handles::const_iterator it = m_handles.begin(); it != hend; ++it)
	{
		if ((*it)->hitTest(point)) {
			return (*it)->dir();
		}
	}
	return SizeHandleRect::None;
}

void ItemBase::updateGeometry()
{
	const QRectF &rect = rectBox();

	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
		SizeHandleRect *hndl = *it;;
		switch (hndl->dir()) {
		case SizeHandleRect::LeftTop:
			hndl->move(rect.x() - selection_handle_size / 2, rect.y() - selection_handle_size / 2);
			break;
		case SizeHandleRect::Top:
			hndl->move(rect.x() + rect.width() / 2 - selection_handle_size / 2, rect.y() - selection_handle_size / 2);
			break;
		case SizeHandleRect::RightTop:
			hndl->move(rect.x() + rect.width() - selection_handle_size / 2, rect.y() - selection_handle_size / 2);
			break;
		case SizeHandleRect::Right:
			hndl->move(rect.x() + rect.width() - selection_handle_size / 2, rect.y() + rect.height() / 2 - selection_handle_size / 2);
			break;
		case SizeHandleRect::RightBottom:
			hndl->move(rect.x() + rect.width() - selection_handle_size / 2, rect.y() + rect.height() - selection_handle_size / 2);
			break;
		case SizeHandleRect::Bottom:
			hndl->move(rect.x() + rect.width() / 2 - selection_handle_size / 2, rect.y() + rect.height() - selection_handle_size / 2);
			break;
		case SizeHandleRect::LeftBottom:
			hndl->move(rect.x() - selection_handle_size / 2, rect.y() + rect.height() - selection_handle_size / 2);
			break;
		case SizeHandleRect::Left:
			hndl->move(rect.x() - selection_handle_size / 2, rect.y() + rect.height() / 2 - selection_handle_size / 2);
			break;
		default:
			break;
		}
	}
}

void ItemBase::setState(SelectionHandleState st)
{
	const Handles::iterator hend = m_handles.end();
	for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
		(*it)->setState(st);
}

QRectF ItemBase::rectBox() const
{
	return QRectF(QPointF(m_rect.left(), m_rect.top()),
		QPointF(m_rect.right(), m_rect.bottom()));
}

void ItemBase::checkSizeHandleRectContent()
{
	for (auto itemSizeH : m_handles)
	{
		QPointF sizeH = itemSizeH->scenePos();
		for (auto var : itemSizeH->m_listSizeHandle)
		{
			QPointF line = var->scenePos();
			//qDebug() << "old line -> " << line << endl;
			if (sizeH != line)
			{
				var->setConnectStatus(NotConnected);
				itemSizeH->m_listSizeHandle.removeOne(var);
				continue;
			}
		}
	}
}

void ItemBase::updateLine()
{
	//遍历图元的每个SizeHandleRect
	for (auto itemSizeH : m_handles)
	{
		//遍历每个SizeHandleRect中存放的线条端点
		for (auto var : itemSizeH->m_listSizeHandle)
		{
			//刷新线条端点坐标，更新线条
			QPointF point = itemSizeH->sceneBoundingRect().center();
			emit var->adsorbentChange(var, point);
			QPointF p = var->parentItem()->mapFromScene(itemSizeH->scenePos());;
			var->move(p.x(), p.y());
			qDebug() << "line total -> " << itemSizeH->m_listSizeHandle.count() << endl;
		}
	}
}
