#include "dragbutton.h"
#include "drawscene.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QByteArray>

DragButton::DragButton(QWidget* parent)
    : QPushButton(parent)
{

}

DragButton::DragButton(const QIcon &icon, const QString &text, QWidget* parent)
    : QPushButton(icon, text, parent)
{

}

void DragButton::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_dragStartPoint = e->pos();
    }
    QPushButton::mousePressEvent(e);
}


void DragButton::mouseMoveEvent(QMouseEvent* e)
{
    if ((e->pos() - m_dragStartPoint).manhattanLength() > QApplication::startDragDistance())
    {
        QDrag* dg = new QDrag(this);
        QPixmap pixmap = grab();
        dg->setDragCursor(pixmap, Qt::DropAction::MoveAction);
        QMimeData* md = new QMimeData;
        // todo DrawScene::Model::enBearItem需要存放在成员变量里
        md->setData("Model", QByteArray::number(DrawScene::Model::enBearItem, 10));
        dg->setMimeData(md);
        dg->exec();
    }
    QPushButton::mouseMoveEvent(e);
}
