#ifndef ITEMLISTVIEW_H
#define ITEMLISTVIEW_H

#include <QListView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include "drawscene.h"

class ItemData
{
public:
    QString m_name;
    QString m_iconPath;
    DrawScene::Model m_type;
};

class ItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ItemModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QMimeData* mimeData(const QModelIndexList &indexes) const override;

    void insertItem(const QList<ItemData> &list);

private:
    QList<ItemData> m_data;
};

class ItemListView : public QListView
{
    Q_OBJECT
public:
    ItemListView(QWidget* parent = nullptr);

    ItemModel* model();
protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

private:
    QPoint              m_dragStartPoint;
    ItemModel*          m_model = nullptr;
};

class ItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ItemDelegate(QObject* parent = nullptr);

protected:
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    void paint(QPainter* painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // ITEMLISTVIEW_H
