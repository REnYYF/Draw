#ifndef COMMANDS
#define COMMANDS

#include <QUndoCommand>
#include <QPair>
#include "drawscene.h"

class MoveShapeCommand : public QUndoCommand
{
public:
	enum { Id = 1234 };
	MoveShapeCommand(QList<QGraphicsItem *> item, QUndoCommand *parent = 0);
	void undo() Q_DECL_OVERRIDE;
	void redo() Q_DECL_OVERRIDE;

	bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
	int id() const Q_DECL_OVERRIDE { return Id; }
private:
	QList<QPair<QGraphicsItem *, QPointF>>	m_oldShap;
	QList<QPair<QGraphicsItem *, QPointF>>	m_newShap;
};

class ResizeShapeCommand : public QUndoCommand
{
public:
    enum { Id = 1235 };
    ResizeShapeCommand(QGraphicsItem * item , const QRectF &rect, QUndoCommand *parent = 0 );
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    int id() const Q_DECL_OVERRIDE { return Id; }
private:
    QGraphicsItem  *m_myItem;
	QRectF			m_oldRect;
	QRectF			m_newRect;
};


class RotateShapeCommand : public QUndoCommand
{
public:
	enum { Id = 1236 };
    RotateShapeCommand(QGraphicsItem *item , const qreal oldAngle ,
                QUndoCommand * parent = 0);
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

	bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
	int id() const Q_DECL_OVERRIDE { return Id; }
private:
    QGraphicsItem *myItem;
    qreal myOldAngle;
    qreal newAngle;
};

class RemoveShapeCommand : public QUndoCommand
{
public:
    explicit RemoveShapeCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = 0);
    ~RemoveShapeCommand();
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    QList<QGraphicsItem *> items;
    QGraphicsScene *myGraphicsScene;
};


class AddShapeCommand : public QUndoCommand
{
public:
    AddShapeCommand(QGraphicsItem *item , QGraphicsScene *graphicsScene,
               QUndoCommand *parent = 0);
    ~AddShapeCommand();

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    QGraphicsItem *myDiagramItem;
    QGraphicsScene *myGraphicsScene;
    QPointF initialPosition;
};

class LineSizeCommand : public QUndoCommand
{
public:
	LineSizeCommand(QGraphicsScene *graphicsScene, const qreal newSize, QUndoCommand *parent = 0);
	~LineSizeCommand();

	void undo() Q_DECL_OVERRIDE;
	void redo() Q_DECL_OVERRIDE;
private:
	qreal									 m_newSize;
	QList<QPair<QGraphicsItem *, qreal>>	 m_oldSize;
	QGraphicsScene							*m_myGraphicsScene;
};

class BrushColorCommand : public QUndoCommand
{
public:
	BrushColorCommand(QGraphicsScene *graphicsScene, const QColor newColor, QUndoCommand *parent = 0);
	~BrushColorCommand();

	void undo() Q_DECL_OVERRIDE;
	void redo() Q_DECL_OVERRIDE;
private:
	QColor									 m_newColor;
	QList<QPair<QGraphicsItem *, QColor>>	 m_oldColor;
	QGraphicsScene							*m_myGraphicsScene;
};

class LineColorCommand :public QUndoCommand
{
public:
	LineColorCommand(QGraphicsScene *graphicsScene, const QColor newColor, QUndoCommand *parent = 0);
	~LineColorCommand();

	void undo() Q_DECL_OVERRIDE;
	void redo() Q_DECL_OVERRIDE;
private:
	QColor									 m_newColor;
	QList<QPair<QGraphicsItem *, QColor>>	 m_oldColor;
	QGraphicsScene							*m_myGraphicsScene;
};

QString createCommandString(QGraphicsItem *item, const QPointF &point);

#endif // COMMANDS

