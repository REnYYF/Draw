#include "drawscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QGraphicsView>
#include <QMimeData>


SelectMode selectMode = none;

SizeHandleRect::Direction nDragHandle = SizeHandleRect::None;

static qreal odlAngle = 0;

DrawScene::DrawScene(QObject* parent)
    : QGraphicsScene(parent),
      m_myMode(enNone),
      m_isRotate(false)
{
    //setBackgroundBrush(QPixmap(":/icons/background2.png"));
    //    setAcceptDrops(true);
}

DrawScene::~DrawScene()
{

}


void DrawScene::setModel(Model modelType)
{
    m_myMode = modelType;
}

void DrawScene::setRotateState(bool state)
{
    m_isRotate = state;
    m_myMode = enNone;
}

void DrawScene::copyItem()
{
    m_listItems.clear();
    m_listItems = selectedItems();
}

void DrawScene::pasteItem()
{
    if (!m_listItems.isEmpty())
    {
        for each(auto var in m_listItems)
        {
            ItemBase* item = acquireItemType(var);
            QGraphicsItem* newItem = item->duplicate();
            emit itemAdded(newItem);
            switch (newItem->type())
            {
                case ELLIPSE_ITEM_TYPE:
                    connect((EllipseItem*)newItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                    break;
                case RECT_ITEM_TYPE:
                    connect((RectItem*)newItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                    break;
                case LINE_ITEM_TYPE:
                    connect((LineItem*)newItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                    break;
                case SVG_ITEM_TYPE:
                    connect((DrawSVGItem*)newItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                    break;
                default:
                    qDebug() << "no shape" << endl;
                    break;
            }
            //addItem(newItem);
        }
        update();
    }
}

void DrawScene::shearItem()
{
    copyItem();
}

void DrawScene::saveFileToXml(const QString &fileName, QList<QGraphicsItem*> items)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "Open File,Error\n";
        return;
    }

    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement Draw = doc.createElement("Draw");
    doc.appendChild(Draw);

    for each(auto var in items)
    {
        ItemBase* itemBase = acquireItemType(var);
        itemBase->saveFileToXml(doc, Draw);
    }

    QTextStream out_stream(&file);
    doc.save(out_stream, 4);
    file.close();
}

void DrawScene::openFileFromXml(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement rootElem = doc.documentElement();
    QDomNode n = rootElem.firstChild();
    while (!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            if (e.nodeName() == "Ellipse")
            {
                EllipseItem* ellipseItem = new EllipseItem(QPointF(0, 0));
                connect(ellipseItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                ellipseItem->loadFileFormXml(e);
                addItem(ellipseItem);
                update();
            }
            if (e.nodeName() == "Rectangle")
            {
                RectItem* rectItem = new RectItem(QPointF(0, 0));
                connect(rectItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                rectItem->loadFileFormXml(e);
                addItem(rectItem);
                update();
            }
            if (e.nodeName() == "Line")
            {
                LineItem* lineItem = new LineItem(QPointF(0, 0));
                connect(lineItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                lineItem->loadFileFormXml(e);
                addItem(lineItem);
                update();
            }
            if (e.nodeName() == "SvgGraphics")
            {
                QString path = e.attribute("path");
                DrawSVGItem* svgItem = new DrawSVGItem(QPointF(0, 0), path);
                connect(svgItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
                svgItem->loadFileFormXml(e);
                addItem(svgItem);
                update();
            }
        }
        n = n.nextSibling();
    }
}

void DrawScene::saveFileToJson(const QString &fileName, QList<QGraphicsItem*> items)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "Open File,Error\n";
        return;
    }

    QJsonArray jsonArr;

    for each(auto var in items)
    {
        ItemBase* itemBase = acquireItemType(var);
        itemBase->saveFileToJson(jsonArr);
    }

    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArr);

    file.write(jsonDoc.toJson());
    file.close();
}

void DrawScene::openFileFromJson(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("fail to open the file: %1, %2, %3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__);
        return;
    }
    QByteArray array = file.readAll();
    file.close();
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument(QJsonDocument::fromJson(array, &jsonParseError));
    if (QJsonParseError::NoError != jsonParseError.error)
    {
        qDebug() << QString("JsonParseError: %1").arg(jsonParseError.errorString());
        return;
    }
    QJsonArray jsonArr = jsonDocument.array();
    for (auto iter = jsonArr.constBegin(); iter != jsonArr.constEnd(); ++iter)
    {
        QJsonObject obj = iter->toObject();
        if (obj["name"] == "Ellipse")
        {
            EllipseItem* ellipseItem = new EllipseItem(QPointF(0, 0));
            connect(ellipseItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            ellipseItem->loadFileFormJson(obj);
            addItem(ellipseItem);
            update();
        }
        if (obj["name"] == "Rectangle")
        {
            RectItem* rectItem = new RectItem(QPointF(0, 0));
            connect(rectItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            rectItem->loadFileFormJson(obj);
            addItem(rectItem);
            update();
        }
        if (obj["name"] == "Line")
        {
            LineItem* lineItem = new LineItem(QPointF(0, 0));
            connect(lineItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            lineItem->loadFileFormJson(obj);
            addItem(lineItem);
            update();
        }
        if (obj["name"] == "SvgGraphics")
        {

            DrawSVGItem* svgItem = new DrawSVGItem(QPointF(0, 0), obj["path"].toString());
            connect(svgItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            svgItem->loadFileFormJson(obj);
            addItem(svgItem);
            update();
        }

    }
}

void DrawScene::setSelectMode(QGraphicsSceneMouseEvent* event)
{
    QList<QGraphicsItem*> items = this->selectedItems();
    ItemBase* itemBase = nullptr;
    QList<QGraphicsView*> viewList = views();
    QGraphicsView* view = viewList.first();
    if (items.count() == 1)
    {
        m_item = items.first();
        itemBase = acquireItemType(m_item);
        if (!itemBase)
        {
            return;
        }
        nDragHandle = itemBase->hitTest(event->scenePos());
        if (nDragHandle != SizeHandleRect::None)
        {
            if (m_isRotate)
            {

                view->setDragMode(QGraphicsView::NoDrag);
                setCursor(QCursor(QPixmap(":/icons/rotate.png")));
                QPointF origin = m_item->mapToScene(itemBase->getRect().center());

                qreal len_y = event->scenePos().y() - origin.y();
                qreal len_x = event->scenePos().x() - origin.x();

                qreal angle = atan2(len_y, len_x) * 180 / M_PI;

                itemBase->m_lastAngle = angle;
                odlAngle = itemBase->m_angle;
                selectMode = rotate;
            }
            else
            {
                view->setDragMode(QGraphicsView::NoDrag);
                setCursor(QCursor(Qt::CrossCursor));
                selectMode = size;
            }
        }
        else
        {
            selectMode = move;
        }
    }
    else
    {
        selectMode = move;
    }
    if (selectMode == move || selectMode == none)
    {
        QGraphicsScene::mousePressEvent(event);
    }
}

void DrawScene::resizeItem(QGraphicsSceneMouseEvent* event)
{
    QList<QGraphicsItem*> items = this->selectedItems();
    if (items.count() == 1)
    {
        ItemBase* item = acquireItemType(items.first());

        if (nDragHandle != SizeHandleRect::None && selectMode == size)
        {
            QList<QGraphicsView*> viewList = views();
            QGraphicsView* view = viewList.first();
            view->setDragMode(QGraphicsView::NoDrag);
            item->resizeTo(nDragHandle, event->scenePos());
            emit itemResize(items.first(), item->m_rect);
            emit itemsChange(items.first());
        }
        if (nDragHandle == SizeHandleRect::None && selectMode == none)
        {
            SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
            if (handle != SizeHandleRect::None)
            {
                //setCursor(item->getCursor(handle));
                setCursor(QCursor(Qt::CrossCursor));
                QGraphicsScene::mouseMoveEvent(event);
            }
            else
            {
                setCursor(Qt::ArrowCursor);
            }
        }
        if (nDragHandle == SizeHandleRect::None && selectMode == move)
        {
            emit itemsChange(items.first());
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
    else
    {
        //emit itemsChange(items.first());
        QGraphicsScene::mouseMoveEvent(event);
    }
    this->update();
}

void DrawScene::rotateItem(QGraphicsSceneMouseEvent* event)
{
    QList<QGraphicsItem*> items = this->selectedItems();
    if (items.count() == 1)
    {
        ItemBase* item = acquireItemType(items.first());

        if (nDragHandle != SizeHandleRect::None && selectMode == rotate)
        {
            item->rotateTo(items.first(), event->scenePos());
            emit itemsChange(items.first());
        }
        if (nDragHandle == SizeHandleRect::None && selectMode == none)
        {
            SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
            if (handle != SizeHandleRect::None)
            {
                //setCursor(item->getCursor(handle));
                setCursor(QCursor(QPixmap(":/icons/rotate.png")));
                QGraphicsScene::mouseMoveEvent(event);
            }
            else
            {
                setCursor(Qt::ArrowCursor);
            }
        }
        if (nDragHandle == SizeHandleRect::None && selectMode == move)
        {
            emit itemsChange(items.first());
            QGraphicsScene::mouseMoveEvent(event);
        }
    }
    else
    {
        //emit itemsChange(items.first());
        QGraphicsScene::mouseMoveEvent(event);
    }
    this->update();
}

void DrawScene::drawItem(QGraphicsSceneMouseEvent* mouseEvent)
{
    if ((m_myMode == Model::enRectItem) && (m_item != nullptr))
    {
        m_rectItem->setEndPoint(mouseEvent->scenePos());
        m_rectItem->updateGeometry();
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    if ((m_myMode == Model::enLineItem) && (m_item != nullptr))
    {
        m_lineItem->setEndPoint(mouseEvent->scenePos());
        m_lineItem->updateGeometry();
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    if ((m_myMode == Model::enEllipseItem) && (m_item != nullptr))
    {
        m_ellipseItem->setEndPoint(mouseEvent->scenePos());
        m_ellipseItem->updateGeometry();
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void DrawScene::setCursor(const QCursor &cursor)
{
    QList<QGraphicsView*> views = this->views();
    if (views.count() > 0)
    {
        QGraphicsView* view = views.first();
        view->setCursor(cursor);
    }
}

void DrawScene::onItemsChange(QGraphicsItem* item)
{
    //qDebug() << "onItemSelect" << item << endl;
    emit itemsChange(item);
}

void DrawScene::createItem(QPointF point, DrawScene::Model modeType)
{
    this->clearSelection();
    switch (modeType)
    {
        case Model::enBearItem:
            m_item = new DrawSVGItem(point, ":/icons/bear.svg");
            connect((DrawSVGItem*)m_item, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            break;
        case Model::enStarItem:
            m_item = new DrawSVGItem(point, ":/icons/star.svg");
            connect((DrawSVGItem*)m_item, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            break;
        case Model::enCartoonFace:
            m_item = new DrawSVGItem(point, ":/icons/CartoonFace.svg");
            connect((DrawSVGItem*)m_item, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            break;
        case Model::enLineItem:
            m_lineItem = new LineItem(point);
            connect(m_lineItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            m_item = m_lineItem;
            break;
        case Model::enRectItem:
            m_rectItem = new RectItem(point);
            connect(m_rectItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            m_item = m_rectItem;
            break;
        case Model::enEllipseItem:
            m_ellipseItem = new EllipseItem(point);
            connect(m_ellipseItem, SIGNAL(itemsChange(QGraphicsItem*)), this, SLOT(onItemsChange(QGraphicsItem*)));
            m_item = m_ellipseItem;
            break;
        default:
            qDebug() << "void DrawScene::createItem(DrawScene::Mode modeType),No modelType" << endl;

    }
    if (m_item)
    {
        emit itemAdded(m_item);
    }
}

void DrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    m_myMode = enNone;
    nDragHandle = SizeHandleRect::None;
    ItemBase* item = nullptr;
    if (m_item && m_isRotate)
    {
        item = acquireItemType(m_item);
        emit itemRotate(m_item, odlAngle);
    }
    if (selectMode == move)
    {
        QList<QGraphicsItem*> itemList = selectedItems();
        if (!itemList.isEmpty())
        {
            item = acquireItemType(itemList.first());
            if (!item)
            {
                return;
            }
            if (item->m_lastPos != itemList.first()->pos())
            {
                emit itemMoved(itemList);
                qDebug() << "selectModel::move" << endl;
            }
        }
    }
    selectMode = none;
    m_isRotate = false;
    m_item = nullptr;
    QList<QGraphicsView*> viewList = views();
    if (!viewList.isEmpty())
    {
        QGraphicsView* view = viewList.first();
        view->setDragMode(QGraphicsView::RubberBandDrag);
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void DrawScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if ((event->button() == Qt::LeftButton) && (m_myMode != enNone))
    {
        QList<QGraphicsView*> viewList = views();
        if (!viewList.isEmpty())
        {
            QGraphicsView* view = viewList.first();
            view->setDragMode(QGraphicsView::NoDrag);
        }
        selectMode = add;
        createItem(event->scenePos(), m_myMode);
        QGraphicsScene::mousePressEvent(event);
    }
    else
    {
        setSelectMode(event);
    }

}

void DrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    QList<QGraphicsView*> viewList = views();
    QGraphicsView* view = viewList.first();

    if (selectMode == add)
    {
        view->setDragMode(QGraphicsView::NoDrag);
        drawItem(mouseEvent);
    }
    else if (m_isRotate)
    {
        view->setDragMode(QGraphicsView::NoDrag);
        rotateItem(mouseEvent);
    }
    else /*if(selectMode = size)*/
    {
        resizeItem(mouseEvent);
    }
}

void DrawScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    QGraphicsScene::dragEnterEvent(event);
}

void DrawScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    //    QGraphicsScene::dragMoveEvent(event);
    event->accept();
}

void DrawScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* md = event->mimeData();
    if (md)
    {
        auto type = Model(md->data("Model").toInt());
        createItem(event->scenePos(), type);
    }
    QGraphicsScene::dropEvent(event);
}

void DrawScene::drawBackground(QPainter* painter, const QRectF &rect)
{
    int mGridSize = 20;
    QRect r = rect.toRect();
    int xmin = r.left() - r.left() % mGridSize - mGridSize;
    int ymin = r.top() - r.top() % mGridSize - mGridSize;
    int xmax = r.right() - r.right() % mGridSize + mGridSize;
    int ymax = r.bottom() - r.bottom() % mGridSize + mGridSize;
    painter->setRenderHints(QPainter::Antialiasing, true);
    for (int x = xmin; x <= xmax; x += mGridSize)
    {
        painter->drawLine(x, r.top(), x, r.bottom());
    }

    for (int y = ymin; y <= ymax; y += mGridSize)
    {
        painter->drawLine(r.left(), y, r.right(), y);
    }
    qInfo() << "drawBackground" << rect;
}

ItemBase* acquireItemType(QGraphicsItem* item)
{

    ItemBase* tmpItem = nullptr;
    if (item->type() == SVG_ITEM_TYPE)
    {
        tmpItem = (DrawSVGItem*)item;
    }
    if (item->type() == RECT_ITEM_TYPE)
    {
        tmpItem = (RectItem*)item;
    }
    if (item->type() == ELLIPSE_ITEM_TYPE)
    {
        tmpItem = (EllipseItem*)item;
    }
    if (item->type() == LINE_ITEM_TYPE)
    {
        tmpItem = (LineItem*)item;
    }
    return tmpItem;

}
