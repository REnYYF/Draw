/****************************************************************************
** Meta object code from reading C++ file 'graphicsproperty.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../graphicsproperty.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphicsproperty.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GraphicsProperty_t {
    QByteArrayData data[22];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphicsProperty_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphicsProperty_t qt_meta_stringdata_GraphicsProperty = {
    {
QT_MOC_LITERAL(0, 0, 16), // "GraphicsProperty"
QT_MOC_LITERAL(1, 17, 12), // "valuesChange"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "itemRotate"
QT_MOC_LITERAL(4, 42, 14), // "QGraphicsItem*"
QT_MOC_LITERAL(5, 57, 4), // "item"
QT_MOC_LITERAL(6, 62, 8), // "oldAngle"
QT_MOC_LITERAL(7, 71, 12), // "itemLineSize"
QT_MOC_LITERAL(8, 84, 15), // "QGraphicsScene*"
QT_MOC_LITERAL(9, 100, 5), // "scene"
QT_MOC_LITERAL(10, 106, 7), // "newSize"
QT_MOC_LITERAL(11, 114, 13), // "itemLineColor"
QT_MOC_LITERAL(12, 128, 8), // "newColor"
QT_MOC_LITERAL(13, 137, 14), // "itemBrushColor"
QT_MOC_LITERAL(14, 152, 9), // "itemMoved"
QT_MOC_LITERAL(15, 162, 21), // "QList<QGraphicsItem*>"
QT_MOC_LITERAL(16, 184, 10), // "itemResize"
QT_MOC_LITERAL(17, 195, 16), // "slotValueChanged"
QT_MOC_LITERAL(18, 212, 11), // "QtProperty*"
QT_MOC_LITERAL(19, 224, 8), // "property"
QT_MOC_LITERAL(20, 233, 3), // "var"
QT_MOC_LITERAL(21, 237, 13) // "onItemsChange"

    },
    "GraphicsProperty\0valuesChange\0\0"
    "itemRotate\0QGraphicsItem*\0item\0oldAngle\0"
    "itemLineSize\0QGraphicsScene*\0scene\0"
    "newSize\0itemLineColor\0newColor\0"
    "itemBrushColor\0itemMoved\0QList<QGraphicsItem*>\0"
    "itemResize\0slotValueChanged\0QtProperty*\0"
    "property\0var\0onItemsChange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphicsProperty[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    2,   60,    2, 0x06 /* Public */,
       7,    2,   65,    2, 0x06 /* Public */,
      11,    2,   70,    2, 0x06 /* Public */,
      13,    2,   75,    2, 0x06 /* Public */,
      14,    1,   80,    2, 0x06 /* Public */,
      16,    2,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    2,   88,    2, 0x0a /* Public */,
      21,    1,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::QReal,    5,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QReal,    9,   10,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QColor,    9,   12,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QColor,    9,   12,
    QMetaType::Void, 0x80000000 | 15,    2,
    QMetaType::Void, 0x80000000 | 4, QMetaType::QRectF,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 18, QMetaType::QVariant,   19,   20,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void GraphicsProperty::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GraphicsProperty *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valuesChange(); break;
        case 1: _t->itemRotate((*reinterpret_cast< QGraphicsItem*(*)>(_a[1])),(*reinterpret_cast< const qreal(*)>(_a[2]))); break;
        case 2: _t->itemLineSize((*reinterpret_cast< QGraphicsScene*(*)>(_a[1])),(*reinterpret_cast< const qreal(*)>(_a[2]))); break;
        case 3: _t->itemLineColor((*reinterpret_cast< QGraphicsScene*(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 4: _t->itemBrushColor((*reinterpret_cast< QGraphicsScene*(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 5: _t->itemMoved((*reinterpret_cast< QList<QGraphicsItem*>(*)>(_a[1]))); break;
        case 6: _t->itemResize((*reinterpret_cast< QGraphicsItem*(*)>(_a[1])),(*reinterpret_cast< const QRectF(*)>(_a[2]))); break;
        case 7: _t->slotValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 8: _t->onItemsChange((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsScene* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsScene* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsScene* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QGraphicsItem*> >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GraphicsProperty::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphicsProperty::valuesChange)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GraphicsProperty::*)(QGraphicsItem * , const qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphicsProperty::itemRotate)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GraphicsProperty::*)(QGraphicsScene * , const qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphicsProperty::itemLineSize)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GraphicsProperty::*)(QGraphicsScene * , const QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphicsProperty::itemLineColor)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GraphicsProperty::*)(QGraphicsScene * , const QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphicsProperty::itemBrushColor)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (GraphicsProperty::*)(QList<QGraphicsItem*> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphicsProperty::itemMoved)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (GraphicsProperty::*)(QGraphicsItem * , const QRectF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphicsProperty::itemResize)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GraphicsProperty::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_GraphicsProperty.data,
    qt_meta_data_GraphicsProperty,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GraphicsProperty::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphicsProperty::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphicsProperty.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GraphicsProperty::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void GraphicsProperty::valuesChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GraphicsProperty::itemRotate(QGraphicsItem * _t1, const qreal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GraphicsProperty::itemLineSize(QGraphicsScene * _t1, const qreal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GraphicsProperty::itemLineColor(QGraphicsScene * _t1, const QColor _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GraphicsProperty::itemBrushColor(QGraphicsScene * _t1, const QColor _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GraphicsProperty::itemMoved(QList<QGraphicsItem*> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GraphicsProperty::itemResize(QGraphicsItem * _t1, const QRectF _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
