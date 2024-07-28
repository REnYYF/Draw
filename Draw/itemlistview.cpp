#include "itemlistview.h"
#include "drawscene.h"
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QDrag>
#include <QApplication>
#include <QMimeData>

ItemModel::ItemModel(QObject* parent)
    : QAbstractListModel(parent)
{

}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    QVariant value;
    switch (role)
    {
        case Qt::ItemDataRole::DecorationRole:
            value = m_data.at(index.row()).m_iconPath;
            break;
        case Qt::ItemDataRole::DisplayRole:
            value = m_data.at(index.row()).m_name;
            break;
        default:
            break;
    }
    return value;
}

QMimeData* ItemModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.isEmpty())
    {
        return QAbstractListModel::mimeData(indexes);
    }
    QMimeData* md = new QMimeData;
    md->setData("CursorImgPath", indexes.first().data(Qt::ItemDataRole::DecorationRole).toString().toUtf8());
    md->setData("Model", QByteArray::number(m_data.at(indexes.first().row()).m_type, 10));
    return md;
}

void ItemModel::insertItem(const QList<ItemData> &list)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size() + list.size() - 1);
    m_data.append(list);
    endInsertRows();
}

ItemListView::ItemListView(QWidget* parent)
    : QListView(parent)
{
    m_model = new ItemModel(this);
    setItemDelegate(new ItemDelegate(this));
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setDragEnabled(true);
    setContentsMargins(0, 0, 0, 0);
}

ItemModel* ItemListView::model()
{
    return m_model;
}

void ItemListView::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_dragStartPoint = e->pos();
    }
    QListView::mousePressEvent(e);
}

void ItemListView::mouseMoveEvent(QMouseEvent* e)
{
    if ((e->pos() - m_dragStartPoint).manhattanLength() > QApplication::startDragDistance())
    {
        if (selectedIndexes().isEmpty())
        {
            return QListView::mouseMoveEvent(e);
        }
        QDrag* dg = new QDrag(this);
        auto path = QString::fromStdString(m_model->mimeData(selectedIndexes())->data("CursorImgPath").toStdString());
        dg->setDragCursor(QPixmap(path).scaled(80, 80), Qt::DropAction::MoveAction);
        dg->setMimeData(m_model->mimeData(selectedIndexes()));
        dg->exec();
    }
    QListView::mouseMoveEvent(e);
}

ItemDelegate::ItemDelegate(QObject* parent)
    : QItemDelegate(parent)
{

}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(70, 90);
}

void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto path = index.model()->data(index, Qt::ItemDataRole::DecorationRole);
    QSvgRenderer svgRender;
    svgRender.load(path.toString());
    svgRender.render(painter, option.rect.adjusted(0, 0, 0, -20));

    auto text = index.model()->data(index, Qt::ItemDataRole::DisplayRole).toString();
    painter->drawText(option.rect.adjusted(0, 70, 0, 0), Qt::AlignCenter, text);
}
