/****************************************************************************
** Meta object code from reading C++ file 'DatabaseManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/DatabaseManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DatabaseManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15DatabaseManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto DatabaseManager::qt_create_metaobjectdata<qt_meta_tag_ZN15DatabaseManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DatabaseManager",
        "fridgeUpdated",
        "",
        "shoppingListUpdated",
        "favoritesChanged",
        "addFavorite",
        "title",
        "removeFavorite",
        "isFavorite",
        "getFavorites",
        "addToHistory",
        "getHistory",
        "addRecipe",
        "ingredients",
        "tags",
        "calories",
        "proteins",
        "fats",
        "carbs",
        "servings",
        "instructions",
        "recipeExists",
        "deleteRecipe",
        "id",
        "updateRecipe",
        "registerUser",
        "username",
        "password",
        "loginUser",
        "getCategories",
        "getAllTags",
        "toggleFavorite",
        "addToShoppingList",
        "getShoppingList",
        "removeFromShoppingList",
        "name",
        "clearShoppingList",
        "addToFridge",
        "getFridgeList",
        "removeFromFridge",
        "clearFridge"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'fridgeUpdated'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'shoppingListUpdated'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'favoritesChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addFavorite'
        QtMocHelpers::MethodData<bool(const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'removeFavorite'
        QtMocHelpers::MethodData<bool(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'isFavorite'
        QtMocHelpers::MethodData<bool(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'getFavorites'
        QtMocHelpers::MethodData<QStringList()>(9, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'addToHistory'
        QtMocHelpers::MethodData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'getHistory'
        QtMocHelpers::MethodData<QStringList()>(11, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'addRecipe'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &, int, double, double, double, int, const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 }, { QMetaType::Int, 15 },
            { QMetaType::Double, 16 }, { QMetaType::Double, 17 }, { QMetaType::Double, 18 }, { QMetaType::Int, 19 },
            { QMetaType::QString, 20 },
        }}),
        // Method 'recipeExists'
        QtMocHelpers::MethodData<bool(const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'deleteRecipe'
        QtMocHelpers::MethodData<bool(int)>(22, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 23 },
        }}),
        // Method 'updateRecipe'
        QtMocHelpers::MethodData<bool(int, const QString &, const QString &, const QString &, int, double, double, double, int, const QString &)>(24, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 23 }, { QMetaType::QString, 6 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 },
            { QMetaType::Int, 15 }, { QMetaType::Double, 16 }, { QMetaType::Double, 17 }, { QMetaType::Double, 18 },
            { QMetaType::Int, 19 }, { QMetaType::QString, 20 },
        }}),
        // Method 'registerUser'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(25, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 26 }, { QMetaType::QString, 27 },
        }}),
        // Method 'loginUser'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 26 }, { QMetaType::QString, 27 },
        }}),
        // Method 'getCategories'
        QtMocHelpers::MethodData<QStringList()>(29, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'getAllTags'
        QtMocHelpers::MethodData<QStringList()>(30, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'toggleFavorite'
        QtMocHelpers::MethodData<bool(const QString &)>(31, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'addToShoppingList'
        QtMocHelpers::MethodData<void(const QString &)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'getShoppingList'
        QtMocHelpers::MethodData<QStringList()>(33, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'removeFromShoppingList'
        QtMocHelpers::MethodData<void(const QString &)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 35 },
        }}),
        // Method 'clearShoppingList'
        QtMocHelpers::MethodData<void()>(36, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addToFridge'
        QtMocHelpers::MethodData<void(const QString &)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 35 },
        }}),
        // Method 'getFridgeList'
        QtMocHelpers::MethodData<QStringList()>(38, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'removeFromFridge'
        QtMocHelpers::MethodData<void(const QString &)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 35 },
        }}),
        // Method 'clearFridge'
        QtMocHelpers::MethodData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DatabaseManager, qt_meta_tag_ZN15DatabaseManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DatabaseManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DatabaseManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DatabaseManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15DatabaseManagerE_t>.metaTypes,
    nullptr
} };

void DatabaseManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DatabaseManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->fridgeUpdated(); break;
        case 1: _t->shoppingListUpdated(); break;
        case 2: _t->favoritesChanged(); break;
        case 3: { bool _r = _t->addFavorite((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->removeFavorite((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->isFavorite((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { QStringList _r = _t->getFavorites();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->addToHistory((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: { QStringList _r = _t->getHistory();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->addRecipe((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[9])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->recipeExists((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->deleteRecipe((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->updateRecipe((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[9])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[10])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->registerUser((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->loginUser((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { QStringList _r = _t->getCategories();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 16: { QStringList _r = _t->getAllTags();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->toggleFavorite((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->addToShoppingList((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: { QStringList _r = _t->getShoppingList();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->removeFromShoppingList((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->clearShoppingList(); break;
        case 22: _t->addToFridge((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: { QStringList _r = _t->getFridgeList();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->removeFromFridge((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->clearFridge(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DatabaseManager::*)()>(_a, &DatabaseManager::fridgeUpdated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DatabaseManager::*)()>(_a, &DatabaseManager::shoppingListUpdated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DatabaseManager::*)()>(_a, &DatabaseManager::favoritesChanged, 2))
            return;
    }
}

const QMetaObject *DatabaseManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DatabaseManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DatabaseManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DatabaseManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void DatabaseManager::fridgeUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DatabaseManager::shoppingListUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DatabaseManager::favoritesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
