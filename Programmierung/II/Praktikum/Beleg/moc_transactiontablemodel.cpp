/****************************************************************************
** Meta object code from reading C++ file 'transactiontablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "transactiontablemodel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'transactiontablemodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSTransactionTableModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSTransactionTableModelENDCLASS = QtMocHelpers::stringData(
    "TransactionTableModel",
    "refreshData",
    "",
    "ColumnIdentity",
    "Id",
    "LibitemId",
    "PersonId",
    "Timestamp",
    "MediaId",
    "Title",
    "PublicationDate",
    "Publisher",
    "FirstName",
    "LastName",
    "ExtensionName",
    "BirthDate",
    "MaxColumnIdentity"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSTransactionTableModelENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[22];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[3];
    char stringdata5[10];
    char stringdata6[9];
    char stringdata7[10];
    char stringdata8[8];
    char stringdata9[6];
    char stringdata10[16];
    char stringdata11[10];
    char stringdata12[10];
    char stringdata13[9];
    char stringdata14[14];
    char stringdata15[10];
    char stringdata16[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSTransactionTableModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSTransactionTableModelENDCLASS_t qt_meta_stringdata_CLASSTransactionTableModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 21),  // "TransactionTableModel"
        QT_MOC_LITERAL(22, 11),  // "refreshData"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 14),  // "ColumnIdentity"
        QT_MOC_LITERAL(50, 2),  // "Id"
        QT_MOC_LITERAL(53, 9),  // "LibitemId"
        QT_MOC_LITERAL(63, 8),  // "PersonId"
        QT_MOC_LITERAL(72, 9),  // "Timestamp"
        QT_MOC_LITERAL(82, 7),  // "MediaId"
        QT_MOC_LITERAL(90, 5),  // "Title"
        QT_MOC_LITERAL(96, 15),  // "PublicationDate"
        QT_MOC_LITERAL(112, 9),  // "Publisher"
        QT_MOC_LITERAL(122, 9),  // "FirstName"
        QT_MOC_LITERAL(132, 8),  // "LastName"
        QT_MOC_LITERAL(141, 13),  // "ExtensionName"
        QT_MOC_LITERAL(155, 9),  // "BirthDate"
        QT_MOC_LITERAL(165, 17)   // "MaxColumnIdentity"
    },
    "TransactionTableModel",
    "refreshData",
    "",
    "ColumnIdentity",
    "Id",
    "LibitemId",
    "PersonId",
    "Timestamp",
    "MediaId",
    "Title",
    "PublicationDate",
    "Publisher",
    "FirstName",
    "LastName",
    "ExtensionName",
    "BirthDate",
    "MaxColumnIdentity"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSTransactionTableModelENDCLASS[] = {

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
       4, uint(TransactionTableModel::Id),
       5, uint(TransactionTableModel::LibitemId),
       6, uint(TransactionTableModel::PersonId),
       7, uint(TransactionTableModel::Timestamp),
       8, uint(TransactionTableModel::MediaId),
       9, uint(TransactionTableModel::Title),
      10, uint(TransactionTableModel::PublicationDate),
      11, uint(TransactionTableModel::Publisher),
      12, uint(TransactionTableModel::FirstName),
      13, uint(TransactionTableModel::LastName),
      14, uint(TransactionTableModel::ExtensionName),
      15, uint(TransactionTableModel::BirthDate),
      16, uint(TransactionTableModel::MaxColumnIdentity),

       0        // eod
};

Q_CONSTINIT const QMetaObject TransactionTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_CLASSTransactionTableModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSTransactionTableModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSTransactionTableModelENDCLASS_t,
        // enum 'ColumnIdentity'
        QtPrivate::TypeAndForceComplete<TransactionTableModel::ColumnIdentity, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TransactionTableModel, std::true_type>,
        // method 'refreshData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void TransactionTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TransactionTableModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->refreshData(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *TransactionTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransactionTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSTransactionTableModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int TransactionTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
