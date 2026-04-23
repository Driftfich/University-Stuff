/****************************************************************************
** Meta object code from reading C++ file 'libitemtablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../tablemodel/libitemtablemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'libitemtablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LibItemTableModel_t {
    QByteArrayData data[18];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LibItemTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LibItemTableModel_t qt_meta_stringdata_LibItemTableModel = {
    {
QT_MOC_LITERAL(0, 0, 17), // "LibItemTableModel"
QT_MOC_LITERAL(1, 18, 11), // "refreshData"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "ColumnIdentity"
QT_MOC_LITERAL(4, 46, 2), // "Id"
QT_MOC_LITERAL(5, 49, 7), // "MediaId"
QT_MOC_LITERAL(6, 57, 15), // "AvailableCopies"
QT_MOC_LITERAL(7, 73, 14), // "BorrowedCopies"
QT_MOC_LITERAL(8, 88, 8), // "Location"
QT_MOC_LITERAL(9, 97, 9), // "Condition"
QT_MOC_LITERAL(10, 107, 5), // "Title"
QT_MOC_LITERAL(11, 113, 15), // "PublicationDate"
QT_MOC_LITERAL(12, 129, 10), // "MainArtist"
QT_MOC_LITERAL(13, 140, 9), // "Publisher"
QT_MOC_LITERAL(14, 150, 11), // "Description"
QT_MOC_LITERAL(15, 162, 5), // "Genre"
QT_MOC_LITERAL(16, 168, 9), // "Languages"
QT_MOC_LITERAL(17, 178, 17) // "MaxColumnIdentity"

    },
    "LibItemTableModel\0refreshData\0\0"
    "ColumnIdentity\0Id\0MediaId\0AvailableCopies\0"
    "BorrowedCopies\0Location\0Condition\0"
    "Title\0PublicationDate\0MainArtist\0"
    "Publisher\0Description\0Genre\0Languages\0"
    "MaxColumnIdentity"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LibItemTableModel[] = {

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
       3,    3, 0x0,   14,   25,

 // enum data: key, value
       4, uint(LibItemTableModel::Id),
       5, uint(LibItemTableModel::MediaId),
       6, uint(LibItemTableModel::AvailableCopies),
       7, uint(LibItemTableModel::BorrowedCopies),
       8, uint(LibItemTableModel::Location),
       9, uint(LibItemTableModel::Condition),
      10, uint(LibItemTableModel::Title),
      11, uint(LibItemTableModel::PublicationDate),
      12, uint(LibItemTableModel::MainArtist),
      13, uint(LibItemTableModel::Publisher),
      14, uint(LibItemTableModel::Description),
      15, uint(LibItemTableModel::Genre),
      16, uint(LibItemTableModel::Languages),
      17, uint(LibItemTableModel::MaxColumnIdentity),

       0        // eod
};

void LibItemTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LibItemTableModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->refreshData(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject LibItemTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_LibItemTableModel.data,
    qt_meta_data_LibItemTableModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LibItemTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LibItemTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LibItemTableModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int LibItemTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
