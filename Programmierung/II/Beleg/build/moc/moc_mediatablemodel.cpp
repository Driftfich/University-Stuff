/****************************************************************************
** Meta object code from reading C++ file 'mediatablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../tablemodel/mediatablemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mediatablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MediaTableModel_t {
    QByteArrayData data[10];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MediaTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MediaTableModel_t qt_meta_stringdata_MediaTableModel = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MediaTableModel"
QT_MOC_LITERAL(1, 16, 14), // "ColumnIdentity"
QT_MOC_LITERAL(2, 31, 2), // "Id"
QT_MOC_LITERAL(3, 34, 5), // "Title"
QT_MOC_LITERAL(4, 40, 9), // "Publisher"
QT_MOC_LITERAL(5, 50, 11), // "Description"
QT_MOC_LITERAL(6, 62, 5), // "Genre"
QT_MOC_LITERAL(7, 68, 15), // "PublicationDate"
QT_MOC_LITERAL(8, 84, 12), // "SubclassType"
QT_MOC_LITERAL(9, 97, 17) // "MaxColumnIdentity"

    },
    "MediaTableModel\0ColumnIdentity\0Id\0"
    "Title\0Publisher\0Description\0Genre\0"
    "PublicationDate\0SubclassType\0"
    "MaxColumnIdentity"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MediaTableModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    8,   19,

 // enum data: key, value
       2, uint(MediaTableModel::Id),
       3, uint(MediaTableModel::Title),
       4, uint(MediaTableModel::Publisher),
       5, uint(MediaTableModel::Description),
       6, uint(MediaTableModel::Genre),
       7, uint(MediaTableModel::PublicationDate),
       8, uint(MediaTableModel::SubclassType),
       9, uint(MediaTableModel::MaxColumnIdentity),

       0        // eod
};

void MediaTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject MediaTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_MediaTableModel.data,
    qt_meta_data_MediaTableModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MediaTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MediaTableModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int MediaTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
