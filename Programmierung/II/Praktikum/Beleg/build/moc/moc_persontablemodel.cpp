/****************************************************************************
** Meta object code from reading C++ file 'persontablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../tablemodel/persontablemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'persontablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PersonTableModel_t {
    QByteArrayData data[15];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PersonTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PersonTableModel_t qt_meta_stringdata_PersonTableModel = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PersonTableModel"
QT_MOC_LITERAL(1, 17, 11), // "refreshData"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "ColumnIdentity"
QT_MOC_LITERAL(4, 45, 2), // "Id"
QT_MOC_LITERAL(5, 48, 9), // "FirstName"
QT_MOC_LITERAL(6, 58, 8), // "LastName"
QT_MOC_LITERAL(7, 67, 13), // "ExtensionName"
QT_MOC_LITERAL(8, 81, 9), // "BirthDate"
QT_MOC_LITERAL(9, 91, 6), // "Gender"
QT_MOC_LITERAL(10, 98, 8), // "Location"
QT_MOC_LITERAL(11, 107, 5), // "Email"
QT_MOC_LITERAL(12, 113, 5), // "Phone"
QT_MOC_LITERAL(13, 119, 4), // "Note"
QT_MOC_LITERAL(14, 124, 17) // "MaxColumnIdentity"

    },
    "PersonTableModel\0refreshData\0\0"
    "ColumnIdentity\0Id\0FirstName\0LastName\0"
    "ExtensionName\0BirthDate\0Gender\0Location\0"
    "Email\0Phone\0Note\0MaxColumnIdentity"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PersonTableModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       1,   20, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       3,    3, 0x0,   11,   25,

 // enum data: key, value
       4, uint(PersonTableModel::Id),
       5, uint(PersonTableModel::FirstName),
       6, uint(PersonTableModel::LastName),
       7, uint(PersonTableModel::ExtensionName),
       8, uint(PersonTableModel::BirthDate),
       9, uint(PersonTableModel::Gender),
      10, uint(PersonTableModel::Location),
      11, uint(PersonTableModel::Email),
      12, uint(PersonTableModel::Phone),
      13, uint(PersonTableModel::Note),
      14, uint(PersonTableModel::MaxColumnIdentity),

       0        // eod
};

void PersonTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PersonTableModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->refreshData(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject PersonTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_PersonTableModel.data,
    qt_meta_data_PersonTableModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PersonTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PersonTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PersonTableModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int PersonTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
