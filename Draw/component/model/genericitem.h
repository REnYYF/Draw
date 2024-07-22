#ifndef GENERICITEM_H
#define GENERICITEM_H

#include <QList>

template<class T>
class GenericItem
{
public:
    GenericItem(T data, GenericItem* parent = nullptr)
        : m_data(data), m_parentItem(parent) {}

    // 行数
    int childCount()
    {
        return m_childItems.count();
    }

    // 添加行
    void insertChild(int row, const T &data)
    {
        auto item = new GenericItem<T>(data, this);
        if (row >= childCount())
        {
            m_childItems.append(item);
        }
        else
        {
            m_childItems.insert(row, item);
        }
    }

    // 获取子节点
    GenericItem* childItem(int row)
    {
        if (row < 0 || row >= childCount())
        {
            return nullptr;
        }
        return m_childItems[row];
    }

    // 父节点
    GenericItem* parentItem()
    {
        return m_parentItem;
    }

    // 获取子节点的index
    int childIndex()
    {
        if (m_parentItem)
        {
            return m_parentItem->m_childItems.indexOf(this);
        }
        return -1;
    }

    // 当前节点数据
    T data()
    {
        return m_data;
    }
private:
    QList<GenericItem<T>*>     m_childItems;
    T                          m_data;
    GenericItem<T>*            m_parentItem;
};

#endif // GENERICITEM_H
