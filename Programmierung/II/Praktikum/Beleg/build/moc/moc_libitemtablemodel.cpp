/****************************************************************************
** Meta object code from reading C++ file 'libitemtablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tablemodel/libitemtablemodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'libitemtablemodel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17LibItemTableModelE_t {};
} // unnamed namespace

template <> constexpr inline auto LibItemTableModel::qt_create_metaobjectdata<qt_meta_tag_ZN17LibItemTableModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
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

    QtMocHelpers::UintData qt_methods {
        // Slot 'refreshData'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ColumnIdentity'
        QtMocHelpers::EnumData<ColumnIdentity>(3, 3, QMC::EnumFlags{}).add({
            {    4, ColumnIdentity::Id },
            {    5, ColumnIdentity::MediaId },
            {    6, ColumnIdentity::AvailableCopies },
            {    7, ColumnIdentity::BorrowedCopies },
            {    8, ColumnIdentity::Location },
            {    9, ColumnIdentity::Condition },
            {   10, ColumnIdentity::Title },
            {   11, ColumnIdentity::PublicationDate },
            {   12, ColumnIdentity::Publisher },
            {   13, ColumnIdentity::Description },
            {   14, ColumnIdentity::Genre },
            {   15, ColumnIdentity::Languages },
            {   16, ColumnIdentity::MaxColumnIdentity },
        }),
    };
    return QtMocHelpers::metaObjectData<LibItemTableModel, qt_meta_tag_ZN17LibItemTableModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LibItemTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17LibItemTableModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17LibItemTableModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17LibItemTableModelE_t>.metaTypes,
    nullptr
} };

void LibItemTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LibItemTableModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
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
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17LibItemTableModelE_t>.strings))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
