/****************************************************************************
** Meta object code from reading C++ file 'infopanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ui/infopanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'infopanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InfoPanel_t {
    QByteArrayData data[35];
    char stringdata0[451];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InfoPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InfoPanel_t qt_meta_stringdata_InfoPanel = {
    {
QT_MOC_LITERAL(0, 0, 9), // "InfoPanel"
QT_MOC_LITERAL(1, 10, 13), // "saveRequested"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "modifiedData"
QT_MOC_LITERAL(4, 38, 17), // "editModeCancelled"
QT_MOC_LITERAL(5, 56, 12), // "fieldChanged"
QT_MOC_LITERAL(6, 69, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(7, 86, 4), // "item"
QT_MOC_LITERAL(8, 91, 6), // "column"
QT_MOC_LITERAL(9, 98, 9), // "fieldName"
QT_MOC_LITERAL(10, 108, 8), // "oldValue"
QT_MOC_LITERAL(11, 117, 8), // "newValue"
QT_MOC_LITERAL(12, 126, 23), // "completerSetupRequested"
QT_MOC_LITERAL(13, 150, 10), // "QLineEdit*"
QT_MOC_LITERAL(14, 161, 6), // "editor"
QT_MOC_LITERAL(15, 168, 11), // "QModelIndex"
QT_MOC_LITERAL(16, 180, 5), // "index"
QT_MOC_LITERAL(17, 186, 11), // "displayInfo"
QT_MOC_LITERAL(18, 198, 10), // "jsonObject"
QT_MOC_LITERAL(19, 209, 13), // "resetEditMode"
QT_MOC_LITERAL(20, 223, 12), // "schemaObject"
QT_MOC_LITERAL(21, 236, 13), // "enterEditMode"
QT_MOC_LITERAL(22, 250, 11), // "saveChanges"
QT_MOC_LITERAL(23, 262, 14), // "cancelEditMode"
QT_MOC_LITERAL(24, 277, 12), // "resetButtons"
QT_MOC_LITERAL(25, 290, 21), // "paintDeleteItemButton"
QT_MOC_LITERAL(26, 312, 9), // "QPainter*"
QT_MOC_LITERAL(27, 322, 7), // "painter"
QT_MOC_LITERAL(28, 330, 20), // "QStyleOptionViewItem"
QT_MOC_LITERAL(29, 351, 6), // "option"
QT_MOC_LITERAL(30, 358, 23), // "setupCompleterForEditor"
QT_MOC_LITERAL(31, 382, 13), // "onItemChanged"
QT_MOC_LITERAL(32, 396, 18), // "handleDeleteAction"
QT_MOC_LITERAL(33, 415, 22), // "onHeaderSectionClicked"
QT_MOC_LITERAL(34, 438, 12) // "logicalIndex"

    },
    "InfoPanel\0saveRequested\0\0modifiedData\0"
    "editModeCancelled\0fieldChanged\0"
    "QTreeWidgetItem*\0item\0column\0fieldName\0"
    "oldValue\0newValue\0completerSetupRequested\0"
    "QLineEdit*\0editor\0QModelIndex\0index\0"
    "displayInfo\0jsonObject\0resetEditMode\0"
    "schemaObject\0enterEditMode\0saveChanges\0"
    "cancelEditMode\0resetButtons\0"
    "paintDeleteItemButton\0QPainter*\0painter\0"
    "QStyleOptionViewItem\0option\0"
    "setupCompleterForEditor\0onItemChanged\0"
    "handleDeleteAction\0onHeaderSectionClicked\0"
    "logicalIndex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InfoPanel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    0,   92,    2, 0x06 /* Public */,
       5,    5,   93,    2, 0x06 /* Public */,
      12,    2,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    2,  109,    2, 0x0a /* Public */,
      17,    3,  114,    2, 0x0a /* Public */,
      21,    0,  121,    2, 0x0a /* Public */,
      22,    0,  122,    2, 0x0a /* Public */,
      23,    0,  123,    2, 0x0a /* Public */,
      24,    0,  124,    2, 0x0a /* Public */,
      25,    3,  125,    2, 0x0a /* Public */,
      30,    2,  132,    2, 0x0a /* Public */,
      31,    2,  137,    2, 0x08 /* Private */,
      32,    1,  142,    2, 0x08 /* Private */,
      33,    1,  145,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int, QMetaType::QString, QMetaType::QVariant, QMetaType::QVariant,    7,    8,    9,   10,   11,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,   16,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonObject, QMetaType::Bool,   18,   19,
    QMetaType::Void, QMetaType::QJsonObject, QMetaType::QJsonObject, QMetaType::Bool,   18,   20,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 28, 0x80000000 | 15,   27,   29,   16,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,   16,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int,    7,    8,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Int,   34,

       0        // eod
};

void InfoPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InfoPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->saveRequested((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->editModeCancelled(); break;
        case 2: _t->fieldChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4])),(*reinterpret_cast< const QVariant(*)>(_a[5]))); break;
        case 3: _t->completerSetupRequested((*reinterpret_cast< QLineEdit*(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 4: _t->displayInfo((*reinterpret_cast< const QJsonObject(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->displayInfo((*reinterpret_cast< const QJsonObject(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 6: _t->enterEditMode(); break;
        case 7: _t->saveChanges(); break;
        case 8: _t->cancelEditMode(); break;
        case 9: _t->resetButtons(); break;
        case 10: _t->paintDeleteItemButton((*reinterpret_cast< QPainter*(*)>(_a[1])),(*reinterpret_cast< const QStyleOptionViewItem(*)>(_a[2])),(*reinterpret_cast< const QModelIndex(*)>(_a[3]))); break;
        case 11: _t->setupCompleterForEditor((*reinterpret_cast< QLineEdit*(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 12: _t->onItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->handleDeleteAction((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 14: _t->onHeaderSectionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InfoPanel::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InfoPanel::saveRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (InfoPanel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InfoPanel::editModeCancelled)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (InfoPanel::*)(QTreeWidgetItem * , int , const QString & , const QVariant & , const QVariant & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InfoPanel::fieldChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (InfoPanel::*)(QLineEdit * , const QModelIndex & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InfoPanel::completerSetupRequested)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InfoPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_InfoPanel.data,
    qt_meta_data_InfoPanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *InfoPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InfoPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InfoPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int InfoPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void InfoPanel::saveRequested(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void InfoPanel::editModeCancelled()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void InfoPanel::fieldChanged(QTreeWidgetItem * _t1, int _t2, const QString & _t3, const QVariant & _t4, const QVariant & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void InfoPanel::completerSetupRequested(QLineEdit * _t1, const QModelIndex & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
