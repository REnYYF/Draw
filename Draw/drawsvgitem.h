#ifndef DRAWSVGITEM_H
#define DRAWSVGITEM_H

#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QPainter>
#include <QFile>
#include <QDomNode>
#include "itembase.h"


#define SVG_ITEM_TYPE (UserType + 1)

class DrawSVGItem : public QGraphicsSvgItem , public ItemBase
{
	Q_OBJECT
public:
    DrawSVGItem(const DrawSVGItem *item, QGraphicsItem *parent = nullptr);
    DrawSVGItem(QPointF point,const QString &svgFile,QGraphicsItem *parent = Q_NULLPTR);
    ~DrawSVGItem();

	void initRect();
	void resizeRect(const QRectF rect);
    virtual void setBrushColor(const QColor color) override;
	virtual QColor getBrushColor() const override;
	void setPoint(const QPointF &point);
	void setLineColor(const QColor &color) { Q_UNUSED(color); };
	virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF &point) override;
	virtual void setLineSize(const qreal &size) { Q_UNUSED(size); };
	virtual void setRectInf(const QRectF &rect);
	virtual QGraphicsItem * duplicate() const override;
	virtual void setAngle(const qreal &angle) override;
	virtual void saveFileToXml(QDomDocument &doc, QDomElement &root) override;
	virtual void loadFileFormXml(QDomElement &element) override;
	virtual void saveFileToJson(QJsonArray &jsonArr) override;
	virtual void loadFileFormJson(QJsonObject &jsonObj) override;



signals:
	void itemsChange(QGraphicsItem *item);

private:
    void changeAttributes(const QString &attName, const QString &attValue);
    void changeAttributes(const QString &attName, const int size);
    void recursiveChangeAttributes(const QDomNode &node, const QString &attName, const QString &attValue);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual int type() const override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	virtual QRectF boundingRect() const override;

private:
	QSvgRenderer       *m_svgRender;
	QString             m_svgFile;
	QDomDocument        m_svgXML;
	QPointF             m_point;
	QColor				m_color;
	int					m_shapeSpace;
	enum { Type = SVG_ITEM_TYPE };

};

#endif // DRAWSVGITEM_H
