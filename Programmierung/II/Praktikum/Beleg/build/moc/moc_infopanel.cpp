/****************************************************************************
** Meta object code from reading C++ file 'infopanel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ui/infopanel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'infopanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN9InfoPanelE_t {};
} // unnamed namespace

template <> constexpr inline auto InfoPanel::qt_create_metaobjectdata<qt_meta_tag_ZN9InfoPanelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "InfoPanel",
        "saveRequested",
        "",
        "modifiedData",
        "editModeCancelled",
        "fieldChanged",
        "QTreeWidgetItem*",
        "item",
        "column",
        "fieldName",
        "QVariant",
        "oldValue",
        "newValue",
        "displayInfo",
        "jsonObject",
        "resetEditMode",
        "schemaObject",
        "enterEditMode",
        "saveChanges",
        "cancelEditMode",
        "resetButtons",
        "paintDeleteItemButton",
        "QPainter*",
        "painter",
        "QStyleOptionViewItem",
        "option",
        "QModelIndex",
        "index",
        "onItemChanged",
        "handleDeleteAction",
        "onHeaderSectionClicked",
        "logicalIndex"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'saveRequested'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 3 },
        }}),
        // Signal 'editModeCancelled'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fieldChanged'
        QtMocHelpers::SignalData<void(QTreeWidgetItem *, int, const QString &, const QVariant &, const QVariant &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 }, { QMetaType::Int, 8 }, { QMetaType::QString, 9 }, { 0x80000000 | 10, 11 },
            { 0x80000000 | 10, 12 },
        }}),
        // Slot 'displayInfo'
        QtMocHelpers::SlotData<void(const QJsonObject &, bool)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 14 }, { QMetaType::Bool, 15 },
        }}),
        // Slot 'displayInfo'
        QtMocHelpers::SlotData<void(const QJsonObject &, const QJsonObject &, bool)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 14 }, { QMetaType::QJsonObject, 16 }, { QMetaType::Bool, 15 },
        }}),
        // Slot 'enterEditMode'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'saveChanges'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'cancelEditMode'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'resetButtons'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'paintDeleteItemButton'
        QtMocHelpers::SlotData<void(QPainter *, const QStyleOptionViewItem &, const QModelIndex &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 23 }, { 0x80000000 | 24, 25 }, { 0x80000000 | 26, 27 },
        }}),
        // Slot 'onItemChanged'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 7 }, { QMetaType::Int, 8 },
        }}),
        // Slot 'handleDeleteAction'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'onHeaderSectionClicked'
        QtMocHelpers::SlotData<void(int)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 31 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<InfoPanel, qt_meta_tag_ZN9InfoPanelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject InfoPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9InfoPanelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9InfoPanelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9InfoPanelE_t>.metaTypes,
    nullptr
} };

void InfoPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<InfoPanel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->saveRequested((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 1: _t->editModeCancelled(); break;
        case 2: _t->fieldChanged((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[5]))); break;
        case 3: _t->displayInfo((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->displayInfo((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 5: _t->enterEditMode(); break;
        case 6: _t->saveChanges(); break;
        case 7: _t->cancelEditMode(); break;
        case 8: _t->resetButtons(); break;
        case 9: _t->paintDeleteItemButton((*reinterpret_cast< std::add_pointer_t<QPainter*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QStyleOptionViewItem>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[3]))); break;
        case 10: _t->onItemChanged((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 11: _t->handleDeleteAction((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 12: _t->onHeaderSectionClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (InfoPanel::*)(const QJsonObject & )>(_a, &InfoPanel::saveRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (InfoPanel::*)()>(_a, &InfoPanel::editModeCancelled, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (InfoPanel::*)(QTreeWidgetItem * , int , const QString & , const QVariant & , const QVariant & )>(_a, &InfoPanel::fieldChanged, 2))
            return;
    }
}

const QMetaObject *InfoPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InfoPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9InfoPanelE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int InfoPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void InfoPanel::saveRequested(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void InfoPanel::editModeCancelled()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void InfoPanel::fieldChanged(QTreeWidgetItem * _t1, int _t2, const QString & _t3, const QVariant & _t4, const QVariant & _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5);
}
QT_WARNING_POP
