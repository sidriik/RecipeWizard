/****************************************************************************
** Meta object code from reading C++ file 'RecipeModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/RecipeModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RecipeModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
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
struct qt_meta_tag_ZN11RecipeModelE_t {};
} // unnamed namespace

template <> constexpr inline auto RecipeModel::qt_create_metaobjectdata<qt_meta_tag_ZN11RecipeModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "RecipeModel",
        "searchRecipes",
        "",
        "ingredient",
        "filterByTag",
        "tag",
        "sortRecipes",
        "sortType",
        "refresh",
        "findBestRecipes",
        "ingredientsText",
        "filterByTags",
        "tags",
        "getVal",
        "QVariant",
        "row",
        "roleName",
        "getRowByTitle",
        "title",
        "searchByTitle",
        "text",
        "applyFilters",
        "maxCal",
        "toggleFavoriteInModel",
        "filterByFavorites",
        "titles"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'searchRecipes'
        QtMocHelpers::MethodData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'filterByTag'
        QtMocHelpers::MethodData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'sortRecipes'
        QtMocHelpers::MethodData<void(const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'findBestRecipes'
        QtMocHelpers::MethodData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Method 'filterByTags'
        QtMocHelpers::MethodData<void(const QStringList &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 12 },
        }}),
        // Method 'getVal'
        QtMocHelpers::MethodData<QVariant(int, const QString &)>(13, 2, QMC::AccessPublic, 0x80000000 | 14, {{
            { QMetaType::Int, 15 }, { QMetaType::QString, 16 },
        }}),
        // Method 'getRowByTitle'
        QtMocHelpers::MethodData<int(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 18 },
        }}),
        // Method 'searchByTitle'
        QtMocHelpers::MethodData<void(const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 20 },
        }}),
        // Method 'applyFilters'
        QtMocHelpers::MethodData<void(const QStringList &, int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 12 }, { QMetaType::Int, 22 },
        }}),
        // Method 'toggleFavoriteInModel'
        QtMocHelpers::MethodData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Method 'filterByFavorites'
        QtMocHelpers::MethodData<void(const QStringList &)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 25 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RecipeModel, qt_meta_tag_ZN11RecipeModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject RecipeModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11RecipeModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11RecipeModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11RecipeModelE_t>.metaTypes,
    nullptr
} };

void RecipeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RecipeModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->searchRecipes((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->filterByTag((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->sortRecipes((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->refresh(); break;
        case 4: _t->findBestRecipes((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->filterByTags((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 6: { QVariant _r = _t->getVal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariant*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->getRowByTitle((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->searchByTitle((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->applyFilters((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->toggleFavoriteInModel((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->filterByFavorites((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *RecipeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecipeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11RecipeModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int RecipeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
