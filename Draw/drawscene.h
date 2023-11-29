#ifndef DRAWSCENE_H
#define DRAWSCENE_H

#include <QGraphicsScene>
#include "rectitem.h"
#include "drawsvgitem.h"
#include "ellipseitem.h"
#include "lineitem.h"
#include "itembase.h"

enum SelectMode
{
	none,
	netSelect,
	add,   //添加
	move,  //移动
	size,  //改变大小
	rotate //旋转
};

class DrawScene :public QGraphicsScene
{
    Q_OBJECT
public:
    enum Model { enRectItem, enStarItem, enLineItem, enEllipseItem, enBearItem , enCartoonFace , enNone};

    DrawScene(QObject *parent = 0);
    ~DrawScene();
    void setModel(Model modelType);
	void setRotateState(bool state);
	void copyItem();
	void pasteItem();
	void shearItem();
	void saveFileToXml(const QString &fileName, QList<QGraphicsItem *> items);
	void openFileFromXml(const QString &fileName);
	void saveFileToJson(const QString &fileName, QList<QGraphicsItem *> items);
	void openFileFromJson(const QString &fileName);

signals:
	void itemsChange(QGraphicsItem *item);

	//undo
	void itemAdded(QGraphicsItem *item);
	void itemRotate(QGraphicsItem *item, const qreal oldAngle);
	void itemResize(QGraphicsItem *item, const QRectF rect);
	void itemMoved(QList<QGraphicsItem *> item);
public slots:
	void onItemsChange(QGraphicsItem *item);

private:
    void createItem(QPointF point,Model modeType);
	void setSelectMode(QGraphicsSceneMouseEvent *event);
	void resizeItem(QGraphicsSceneMouseEvent *mouseEvent);
	void rotateItem(QGraphicsSceneMouseEvent *mouseEvent);
	void drawItem(QGraphicsSceneMouseEvent *mouseEvent);
	void setCursor(const QCursor & cursor);

private:
	QList<QGraphicsItem *>		m_listItems;
    Model						m_myMode;
    QGraphicsItem				*m_item;
    RectItem					*m_rectItem;
	LineItem					*m_lineItem;
	EllipseItem					*m_ellipseItem;
	bool						m_isRotate;

     // QGraphicsScene interface
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
};


ItemBase* acquireItemType(QGraphicsItem *item);


#endif // DRAWSCENE_H
