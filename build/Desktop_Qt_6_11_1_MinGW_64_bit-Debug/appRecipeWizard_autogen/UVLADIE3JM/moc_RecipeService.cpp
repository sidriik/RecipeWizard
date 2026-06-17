/****************************************************************************
** Meta object code from reading C++ file 'RecipeService.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/RecipeService.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RecipeService.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13RecipeServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto RecipeService::qt_create_metaobjectdata<qt_meta_tag_ZN13RecipeServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "RecipeService",
        "getAIAnalysis",
        "",
        "title",
        "tags",
        "calories",
        "calculateDailyCalories",
        "weight",
        "height",
        "age",
        "isMale",
        "activityFactor",
        "scaleIngredients",
        "ingredients",
        "multiplier",
        "getCleanName",
        "ingredient",
        "getStepList",
        "instructions",
        "getTimerSeconds",
        "stepText",
        "calculateFullNutrients",
        "activity"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'getAIAnalysis'
        QtMocHelpers::MethodData<QString(const QString &, const QString &, int)>(1, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 }, { QMetaType::Int, 5 },
        }}),
        // Method 'calculateDailyCalories'
        QtMocHelpers::MethodData<double(double, double, int, bool, double)>(6, 2, QMC::AccessPublic, QMetaType::Double, {{
            { QMetaType::Double, 7 }, { QMetaType::Double, 8 }, { QMetaType::Int, 9 }, { QMetaType::Bool, 10 },
            { QMetaType::Double, 11 },
        }}),
        // Method 'scaleIngredients'
        QtMocHelpers::MethodData<QString(const QString &, double)>(12, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 13 }, { QMetaType::Double, 14 },
        }}),
        // Method 'getCleanName'
        QtMocHelpers::MethodData<QString(const QString &)>(15, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 16 },
        }}),
        // Method 'getStepList'
        QtMocHelpers::MethodData<QStringList(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::QStringList, {{
            { QMetaType::QString, 18 },
        }}),
        // Method 'getTimerSeconds'
        QtMocHelpers::MethodData<int(const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 20 },
        }}),
        // Method 'calculateFullNutrients'
        QtMocHelpers::MethodData<QString(double, double, int, bool, double)>(21, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Double, 7 }, { QMetaType::Double, 8 }, { QMetaType::Int, 9 }, { QMetaType::Bool, 10 },
            { QMetaType::Double, 22 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RecipeService, qt_meta_tag_ZN13RecipeServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject RecipeService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13RecipeServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13RecipeServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13RecipeServiceE_t>.metaTypes,
    nullptr
} };

void RecipeService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RecipeService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = _t->getAIAnalysis((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 1: { double _r = _t->calculateDailyCalories((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5])));
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->scaleIngredients((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->getCleanName((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QStringList _r = _t->getStepList((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->getTimerSeconds((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->calculateFullNutrients((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *RecipeService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecipeService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13RecipeServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RecipeService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
