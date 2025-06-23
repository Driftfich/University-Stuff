/****************************************************************************
** Meta object code from reading C++ file 'mediatablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tablemodel/mediatablemodel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mediatablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMediaTableModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMediaTableModelENDCLASS = QtMocHelpers::stringData(
    "MediaTableModel",
    "ColumnIdentity",
    "Id",
    "Title",
    "Publisher",
    "Description",
    "Genre",
    "PublicationDate",
    "SubclassType",
    "MaxColumnIdentity"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMediaTableModelENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[16];
    char stringdata1[15];
    char stringdata2[3];
    char stringdata3[6];
    char stringdata4[10];
    char stringdata5[12];
    char stringdata6[6];
    char stringdata7[16];
    char stringdata8[13];
    char stringdata9[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMediaTableModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMediaTableModelENDCLASS_t qt_meta_stringdata_CLASSMediaTableModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "MediaTableModel"
        QT_MOC_LITERAL(16, 14),  // "ColumnIdentity"
        QT_MOC_LITERAL(31, 2),  // "Id"
        QT_MOC_LITERAL(34, 5),  // "Title"
        QT_MOC_LITERAL(40, 9),  // "Publisher"
        QT_MOC_LITERAL(50, 11),  // "Description"
        QT_MOC_LITERAL(62, 5),  // "Genre"
        QT_MOC_LITERAL(68, 15),  // "PublicationDate"
        QT_MOC_LITERAL(84, 12),  // "SubclassType"
        QT_MOC_LITERAL(97, 17)   // "MaxColumnIdentity"
    },
    "MediaTableModel",
    "ColumnIdentity",
    "Id",
    "Title",
    "Publisher",
    "Description",
    "Genre",
    "PublicationDate",
    "SubclassType",
    "MaxColumnIdentity"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMediaTableModelENDCLASS[] = {

 // content:
      12,       // revision
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

Q_CONSTINIT const QMetaObject MediaTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_CLASSMediaTableModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMediaTableModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMediaTableModelENDCLASS_t,
        // enum 'ColumnIdentity'
        QtPrivate::TypeAndForceComplete<MediaTableModel::ColumnIdentity, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MediaTableModel, std::true_type>
    >,
    nullptr
} };

void MediaTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *MediaTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMediaTableModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int MediaTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
