/****************************************************************************
** Meta object code from reading C++ file 'persontablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tablemodel/persontablemodel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'persontablemodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPersonTableModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSPersonTableModelENDCLASS = QtMocHelpers::stringData(
    "PersonTableModel",
    "refreshData",
    "",
    "ColumnIdentity",
    "Id",
    "FirstName",
    "LastName",
    "ExtensionName",
    "BirthDate",
    "Gender",
    "Location",
    "Email",
    "Phone",
    "Note",
    "MaxColumnIdentity"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPersonTableModelENDCLASS_t {
    uint offsetsAndSizes[30];
    char stringdata0[17];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[3];
    char stringdata5[10];
    char stringdata6[9];
    char stringdata7[14];
    char stringdata8[10];
    char stringdata9[7];
    char stringdata10[9];
    char stringdata11[6];
    char stringdata12[6];
    char stringdata13[5];
    char stringdata14[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPersonTableModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPersonTableModelENDCLASS_t qt_meta_stringdata_CLASSPersonTableModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 16),  // "PersonTableModel"
        QT_MOC_LITERAL(17, 11),  // "refreshData"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 14),  // "ColumnIdentity"
        QT_MOC_LITERAL(45, 2),  // "Id"
        QT_MOC_LITERAL(48, 9),  // "FirstName"
        QT_MOC_LITERAL(58, 8),  // "LastName"
        QT_MOC_LITERAL(67, 13),  // "ExtensionName"
        QT_MOC_LITERAL(81, 9),  // "BirthDate"
        QT_MOC_LITERAL(91, 6),  // "Gender"
        QT_MOC_LITERAL(98, 8),  // "Location"
        QT_MOC_LITERAL(107, 5),  // "Email"
        QT_MOC_LITERAL(113, 5),  // "Phone"
        QT_MOC_LITERAL(119, 4),  // "Note"
        QT_MOC_LITERAL(124, 17)   // "MaxColumnIdentity"
    },
    "PersonTableModel",
    "refreshData",
    "",
    "ColumnIdentity",
    "Id",
    "FirstName",
    "LastName",
    "ExtensionName",
    "BirthDate",
    "Gender",
    "Location",
    "Email",
    "Phone",
    "Note",
    "MaxColumnIdentity"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPersonTableModelENDCLASS[] = {

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
       3,    3, 0x0,   11,   26,

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

Q_CONSTINIT const QMetaObject PersonTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_CLASSPersonTableModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPersonTableModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPersonTableModelENDCLASS_t,
        // enum 'ColumnIdentity'
        QtPrivate::TypeAndForceComplete<PersonTableModel::ColumnIdentity, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PersonTableModel, std::true_type>,
        // method 'refreshData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

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

const QMetaObject *PersonTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PersonTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPersonTableModelENDCLASS.stringdata0))
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
