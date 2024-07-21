#ifndef DRAGBUTTON_H
#define DRAGBUTTON_H

#include <QPushButton>

class DragButton : public QPushButton
{
    Q_OBJECT
public:
    DragButton(QWidget* parent = nullptr);
    DragButton(const QIcon &icon, const QString &text, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;

private:
    QPoint          m_dragStartPoint;
};

#endif // DRAGBUTTON_H
