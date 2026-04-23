/****************************************************************************
** Meta object code from reading C++ file 'libitemtablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "libitemtablemodel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'libitemtablemodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSLibItemTableModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSLibItemTableModelENDCLASS = QtMocHelpers::stringData(
    "LibItemTableModel",
    "refreshData",
    "",
    "ColumnIdentity",
    "Id",
    "MediaId",
    "AvailableCopies",
    "BorrowedCopies",
    "Location",
    "Condition",
    "Title",
    "PublicationDate",
    "Publisher",
    "Description",
    "Genre",
    "Languages",
    "MaxColumnIdentity"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLibItemTableModelENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[18];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[3];
    char stringdata5[8];
    char stringdata6[16];
    char stringdata7[15];
    char stringdata8[9];
    char stringdata9[10];
    char stringdata10[6];
    char stringdata11[16];
    char stringdata12[10];
    char stringdata13[12];
    char stringdata14[6];
    char stringdata15[10];
    char stringdata16[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLibItemTableModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLibItemTableModelENDCLASS_t qt_meta_stringdata_CLASSLibItemTableModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 17),  // "LibItemTableModel"
        QT_MOC_LITERAL(18, 11),  // "refreshData"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 14),  // "ColumnIdentity"
        QT_MOC_LITERAL(46, 2),  // "Id"
        QT_MOC_LITERAL(49, 7),  // "MediaId"
        QT_MOC_LITERAL(57, 15),  // "AvailableCopies"
        QT_MOC_LITERAL(73, 14),  // "BorrowedCopies"
        QT_MOC_LITERAL(88, 8),  // "Location"
        QT_MOC_LITERAL(97, 9),  // "Condition"
        QT_MOC_LITERAL(107, 5),  // "Title"
        QT_MOC_LITERAL(113, 15),  // "PublicationDate"
        QT_MOC_LITERAL(129, 9),  // "Publisher"
        QT_MOC_LITERAL(139, 11),  // "Description"
        QT_MOC_LITERAL(151, 5),  // "Genre"
        QT_MOC_LITERAL(157, 9),  // "Languages"
        QT_MOC_LITERAL(167, 17)   // "MaxColumnIdentity"
    },
    "LibItemTableModel",
    "refreshData",
    "",
    "ColumnIdentity",
    "Id",
    "MediaId",
    "AvailableCopies",
    "BorrowedCopies",
    "Location",
    "Condition",
    "Title",
    "PublicationDate",
    "Publisher",
    "Description",
    "Genre",
    "Languages",
    "MaxColumnIdentity"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLibItemTableModelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       1,   21, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x0a,    2 /* Public */,

 // slots: parameters
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       3,    3, 0x0,   13,   26,

 // enum data: key, value
       4, uint(LibItemTableModel::Id),
       5, uint(LibItemTableModel::MediaId),
       6, uint(LibItemTableModel::AvailableCopies),
       7, uint(LibItemTableModel::BorrowedCopies),
       8, uint(LibItemTableModel::Location),
       9, uint(LibItemTableModel::Condition),
      10, uint(LibItemTableModel::Title),
      11, uint(LibItemTableModel::PublicationDate),
      12, uint(LibItemTableModel::Publisher),
      13, uint(LibItemTableModel::Description),
      14, uint(LibItemTableModel::Genre),
      15, uint(LibItemTableModel::Languages),
      16, uint(LibItemTableModel::MaxColumnIdentity),

       0        // eod
};

Q_CONSTINIT const QMetaObject LibItemTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_CLASSLibItemTableModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLibItemTableModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLibItemTableModelENDCLASS_t,
        // enum 'ColumnIdentity'
        QtPrivate::TypeAndForceComplete<LibItemTableModel::ColumnIdentity, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LibItemTableModel, std::true_type>,
        // method 'refreshData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

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

const QMetaObject *LibItemTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LibItemTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLibItemTableModelENDCLASS.stringdata0))
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
