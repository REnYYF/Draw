#ifndef GRAPHICSPROPERTY_H
#define GRAPHICSPROPERTY_H

#include <QWidget>
#include <QMap>
#include <QString>
#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtvariantproperty.h"
#include "drawscene.h"
class GraphicsProperty : public QWidget
{
    Q_OBJECT
public:
    GraphicsProperty(QWidget* parent = nullptr);

signals:
    void valuesChange();

    //undo redo
    void itemRotate(QGraphicsItem* item, const qreal oldAngle);
    void itemLineSize(QGraphicsScene* scene, const qreal newSize);
    void itemLineColor(QGraphicsScene* scene, const QColor newColor);
    void itemBrushColor(QGraphicsScene* scene, const QColor newColor);
    void itemMoved(QList<QGraphicsItem*>);
    void itemResize(QGraphicsItem*, const QRectF);
public slots :
    void slotValueChanged(QtProperty* property, const QVariant &var);
    void onItemsChange(QGraphicsItem* item);
    //void itemChange();

private:
    void positionProperty(const QPointF &point);
    void penProperty(ItemBase* item);
    void brushProperty(ItemBase* item);
    void sizeProperty(ItemBase* item);
    void angleProperty(ItemBase* item);
    void updateProperty();

private:
    QtTreePropertyBrowser*                 m_propertyEditor;
    QtVariantPropertyManager*              m_varManager;
    QtVariantEditorFactory*                    m_varFactory;
    QGraphicsItem*                         m_item;
    QMap<QtProperty*, QString>                m_propertyToId;
    QMap<QString, QtVariantProperty*>         m_idToProperty;
};

#endif // GRAPHICSPROPERTY_H
