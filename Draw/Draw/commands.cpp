#include "commands.h"
#include <QDebug>

MoveShapeCommand::MoveShapeCommand(QList<QGraphicsItem *> item, QUndoCommand *parent /*= 0*/)
	: QUndoCommand(parent)
{
	ItemBase *itemBase=nullptr;
	for each (auto var in item)
	{
		itemBase = acquireItemType(var);
		if (itemBase)
		{
			m_oldShap.push_back(QPair<QGraphicsItem *, QPointF>(var, itemBase->m_lastPos));
			m_newShap.push_back(QPair<QGraphicsItem *, QPointF>(var, var->pos()));
		}
	}
}

void MoveShapeCommand::undo()
{
	for each (auto var in m_oldShap)
	{
		var.first->setPos(var.second);
	}
	if (!m_oldShap.isEmpty())
	{
		m_oldShap.first().first->scene()->update();
	}
	setText(QObject::tr("undo MoveShap"));
}

void MoveShapeCommand::redo() 
{
	for each (auto var in m_newShap)
	{
		var.first->setPos(var.second);
	}

	setText(QObject::tr("redo MoveShap"));
}


bool MoveShapeCommand::mergeWith(const QUndoCommand *command)
{
	if (command->id() != MoveShapeCommand::Id)
		return false;

	const MoveShapeCommand *cmd = static_cast<const MoveShapeCommand *>(command);

	QGraphicsItem * item = cmd->m_newShap.first().first;

	if (m_newShap.first().first != item)
		return false;

	m_newShap = cmd->m_newShap;

	setText(QObject::tr("mergeWith MoveShape"));
	return true;
}

//! [3]
//! [4]
RemoveShapeCommand::RemoveShapeCommand(QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    items = myGraphicsScene->selectedItems();
}

RemoveShapeCommand::~RemoveShapeCommand()
{

}
//! [4]

//! [5]
void RemoveShapeCommand::undo()
{
    foreach (QGraphicsItem *item, items) {
        QGraphicsItemGroup *g = dynamic_cast<QGraphicsItemGroup*>(item->parentItem());
        if ( !g )
            myGraphicsScene->addItem(item);
    }
    myGraphicsScene->update();
    setText(QObject::tr("Undo Delete %1").arg(items.count()));
}
//! [5]

//! [6]
void RemoveShapeCommand::redo()
{
    foreach (QGraphicsItem *item, items) {
        QGraphicsItemGroup *g = dynamic_cast<QGraphicsItemGroup*>(item->parentItem());
        if ( !g )
            myGraphicsScene->removeItem(item);
    }
        setText(QObject::tr("Redo Delete %1").arg(items.count()));
}
//! [6]

//! [7]
AddShapeCommand::AddShapeCommand(QGraphicsItem *item,
                       QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    myDiagramItem = item;
	initialPosition = item->pos();
}
//! [7]

AddShapeCommand::~AddShapeCommand()
{
    //if (!myDiagramItem->scene())
    //    delete myDiagramItem;
}

//! [8]
void AddShapeCommand::undo()
{
    myGraphicsScene->removeItem(myDiagramItem);
    myGraphicsScene->update();
    setText(QObject::tr("Undo Add %1")
        .arg(createCommandString(myDiagramItem, initialPosition)));

}
//! [8]

//! [9]
void AddShapeCommand::redo()
{
    myGraphicsScene->addItem(myDiagramItem);
    myGraphicsScene->update();
    setText(QObject::tr("Redo Add %1")
        .arg(createCommandString(myDiagramItem, initialPosition)));

}

QString createCommandString(QGraphicsItem *item, const QPointF &pos)
{
	Q_UNUSED(item);
    return QObject::tr("Item at (%1, %2)")
        .arg(pos.x()).arg(pos.y());
}


RotateShapeCommand::RotateShapeCommand(QGraphicsItem *item, const qreal oldAngle, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    myItem = item;
    myOldAngle = oldAngle;
    newAngle = item->rotation();

}

void RotateShapeCommand::undo()
{
    myItem->setRotation(myOldAngle);
    myItem->scene()->update();
    setText(QObject::tr("Undo Rotate %1").arg(newAngle));
}

void RotateShapeCommand::redo()
{
	myItem->setRotation(newAngle);
	myItem->update();
    setText(QObject::tr("Redo Rotate %1").arg(newAngle));
}


bool RotateShapeCommand::mergeWith(const QUndoCommand *command)
{
	if (command->id() != RotateShapeCommand::Id)
		return false;

	const RotateShapeCommand *cmd = static_cast<const RotateShapeCommand *>(command);

	QGraphicsItem *item = cmd->myItem;

	if (myItem != item)
		return false;

	newAngle = cmd->newAngle;

	setText(QObject::tr("mergeWith Rotate"));
	return true;
}

ResizeShapeCommand::ResizeShapeCommand(QGraphicsItem * item, const QRectF &rect, QUndoCommand *parent /*= 0 */)
	: QUndoCommand(parent)
{
	m_oldRect = rect;
	m_myItem = item;
	m_newRect = rect;
}

void ResizeShapeCommand::undo()
{
	ItemBase *item = acquireItemType(m_myItem);
	item->setRectInf(m_oldRect);
	item->updateGeometry();
	m_myItem->scene()->update();
	setText(QObject::tr("undo Resize"));
}

void ResizeShapeCommand::redo()
{
	ItemBase *item = acquireItemType(m_myItem);
	item->setRectInf(m_newRect);
	setText(QObject::tr("redo Resize"));
}
bool ResizeShapeCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != ResizeShapeCommand::Id )
        return false;

    const ResizeShapeCommand *cmd = static_cast<const ResizeShapeCommand *>(command);

    QGraphicsItem *item = cmd->m_myItem;

    if (m_myItem != item)
        return false;

	m_newRect = cmd->m_newRect;

	setText(QObject::tr("mergeWith Resize"));
    return true;
}

LineSizeCommand::LineSizeCommand(QGraphicsScene *graphicsScene, const qreal newSize, QUndoCommand *parent)
	:QUndoCommand(parent)
{
	m_myGraphicsScene = graphicsScene;
	QList<QGraphicsItem *> itemList = m_myGraphicsScene->selectedItems();
	m_newSize = newSize;
	ItemBase *itemBase = nullptr;
	for each (auto var in itemList)
	{
		itemBase = acquireItemType(var);
		m_oldSize.push_back(QPair<QGraphicsItem *, qreal>(var, itemBase->getLineSize()));
	}
}

LineSizeCommand::~LineSizeCommand()
{

}

void LineSizeCommand::undo() 
{
	ItemBase *itemBase = nullptr;
	for each (auto var in m_oldSize)
	{
		itemBase = acquireItemType(var.first);
		itemBase->setLineSize(var.second);
	}
	m_myGraphicsScene->update();

	setText(QObject::tr("Undo LineSize"));
}

void LineSizeCommand::redo() 
{
	ItemBase *itemBase = nullptr;
	for each (auto var in m_oldSize)
	{
		itemBase = acquireItemType(var.first);
		itemBase->setLineSize(m_newSize);
	}
	m_myGraphicsScene->update();

	setText(QObject::tr("Redo LineSize %1").arg(m_newSize));
}

LineColorCommand::LineColorCommand(QGraphicsScene *graphicsScene,const QColor newColor, QUndoCommand *parent /*= 0*/)
	:QUndoCommand(parent)
{
	m_myGraphicsScene = graphicsScene;
	QList<QGraphicsItem *> itemList = m_myGraphicsScene->selectedItems();
	m_newColor = newColor;
	ItemBase *itemBase = nullptr;
	for each (auto var in itemList)
	{
		itemBase = acquireItemType(var);
		m_oldColor.push_back(QPair<QGraphicsItem *, QColor>(var, itemBase->getLineColor()));
	}
}

LineColorCommand::~LineColorCommand()
{

}

void LineColorCommand::undo()
{
	ItemBase *itemBase = nullptr;
	for each (auto var in m_oldColor)
	{
		itemBase = acquireItemType(var.first);
		itemBase->setLineColor(var.second);
	}
	m_myGraphicsScene->update();

	setText(QObject::tr("Undo LineColor"));
}

void LineColorCommand::redo()
{
	ItemBase *itemBase = nullptr;
	for each (auto var in m_oldColor)
	{
		itemBase = acquireItemType(var.first);
		itemBase->setLineColor(m_newColor);
	}
	m_myGraphicsScene->update();

	setText(QObject::tr("Redo LineColor rgba(%1,%2,%3)").arg(m_newColor.red()).arg(m_newColor.green()).arg(m_newColor.blue()).arg(m_newColor.alpha()));
}

BrushColorCommand::BrushColorCommand(QGraphicsScene *graphicsScene, const QColor newColor, QUndoCommand *parent /*= 0*/)
	:QUndoCommand(parent)
{
	m_myGraphicsScene = graphicsScene;
	QList<QGraphicsItem *> itemList= m_myGraphicsScene->selectedItems();
	m_newColor = newColor;
	ItemBase *itemBase = nullptr;
	for each (auto var in itemList)
	{
		itemBase = acquireItemType(var);
		m_oldColor.push_back(QPair<QGraphicsItem *, QColor>(var, itemBase->getBrushColor()));
	}
}

BrushColorCommand::~BrushColorCommand()
{

}

void BrushColorCommand::undo()
{
	ItemBase *itemBase = nullptr;
	for each (auto var in m_oldColor)
	{
		itemBase = acquireItemType(var.first);
		itemBase->setBrushColor(var.second);
	}
	m_myGraphicsScene->update();

	setText(QObject::tr("Undo BrushColor"));
}

void BrushColorCommand::redo()
{
	ItemBase *itemBase = nullptr;
	for each (auto var in m_oldColor)
	{
		itemBase = acquireItemType(var.first);
		itemBase->setBrushColor(m_newColor);
	}
	m_myGraphicsScene->update();

	setText(QObject::tr("Redo BrushColor rgba(%1,%2,%3)").arg(m_newColor.red()).arg(m_newColor.green()).arg(m_newColor.blue()).arg(m_newColor.alpha()));
}
