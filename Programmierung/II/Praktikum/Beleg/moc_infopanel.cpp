/****************************************************************************
** Meta object code from reading C++ file 'infopanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/infopanel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'infopanel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSInfoPanelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSInfoPanelENDCLASS = QtMocHelpers::stringData(
    "InfoPanel",
    "saveRequested",
    "",
    "modifiedData",
    "displayInfo",
    "jsonObject",
    "addJsonToTree",
    "value",
    "QTreeWidgetItem*",
    "parent",
    "enterEditMode",
    "saveChanges",
    "cancelEditMode",
    "onItemChanged",
    "item",
    "column"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSInfoPanelENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[10];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[12];
    char stringdata5[11];
    char stringdata6[14];
    char stringdata7[6];
    char stringdata8[17];
    char stringdata9[7];
    char stringdata10[14];
    char stringdata11[12];
    char stringdata12[15];
    char stringdata13[14];
    char stringdata14[5];
    char stringdata15[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSInfoPanelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSInfoPanelENDCLASS_t qt_meta_stringdata_CLASSInfoPanelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "InfoPanel"
        QT_MOC_LITERAL(10, 13),  // "saveRequested"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 12),  // "modifiedData"
        QT_MOC_LITERAL(38, 11),  // "displayInfo"
        QT_MOC_LITERAL(50, 10),  // "jsonObject"
        QT_MOC_LITERAL(61, 13),  // "addJsonToTree"
        QT_MOC_LITERAL(75, 5),  // "value"
        QT_MOC_LITERAL(81, 16),  // "QTreeWidgetItem*"
        QT_MOC_LITERAL(98, 6),  // "parent"
        QT_MOC_LITERAL(105, 13),  // "enterEditMode"
        QT_MOC_LITERAL(119, 11),  // "saveChanges"
        QT_MOC_LITERAL(131, 14),  // "cancelEditMode"
        QT_MOC_LITERAL(146, 13),  // "onItemChanged"
        QT_MOC_LITERAL(160, 4),  // "item"
        QT_MOC_LITERAL(165, 6)   // "column"
    },
    "InfoPanel",
    "saveRequested",
    "",
    "modifiedData",
    "displayInfo",
    "jsonObject",
    "addJsonToTree",
    "value",
    "QTreeWidgetItem*",
    "parent",
    "enterEditMode",
    "saveChanges",
    "cancelEditMode",
    "onItemChanged",
    "item",
    "column"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSInfoPanelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   59,    2, 0x0a,    3 /* Public */,
       6,    2,   62,    2, 0x0a,    5 /* Public */,
      10,    0,   67,    2, 0x0a,    8 /* Public */,
      11,    0,   68,    2, 0x0a,    9 /* Public */,
      12,    0,   69,    2, 0x0a,   10 /* Public */,
      13,    2,   70,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void, QMetaType::QJsonValue, 0x80000000 | 8,    7,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,   14,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject InfoPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSInfoPanelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSInfoPanelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSInfoPanelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<InfoPanel, std::true_type>,
        // method 'saveRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'displayInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'addJsonToTree'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonValue &, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        // method 'enterEditMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveChanges'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cancelEditMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onItemChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void InfoPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InfoPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->saveRequested((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 1: _t->displayInfo((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 2: _t->addJsonToTree((*reinterpret_cast< std::add_pointer_t<QJsonValue>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[2]))); break;
        case 3: _t->enterEditMode(); break;
        case 4: _t->saveChanges(); break;
        case 5: _t->cancelEditMode(); break;
        case 6: _t->onItemChanged((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InfoPanel::*)(const QJsonObject & );
            if (_t _q_method = &InfoPanel::saveRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *InfoPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InfoPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSInfoPanelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int InfoPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void InfoPanel::saveRequested(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
