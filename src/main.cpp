#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "DatabaseManager.h"
#include "RecipeModel.h"
#include "RecipeService.h"

int main(int argc, char *argv[]) {
    QQuickStyle::setStyle("Basic");
    QGuiApplication app(argc, argv);

    DatabaseManager::instance().init();

    QQmlApplicationEngine engine;

    RecipeRepository repository(QSqlDatabase::database());
    RecipeService service(&repository);
    RecipeModel mainModel(&service);    // Для Главной страницы
    RecipeModel pantryModel(&service);  // Для вкладки Шеф (Холодильник)

    // РЕГИСТРИРУЕМ ТРИ ОБЪЕКТА (БЕЗ НИХ БУДЕТ БЕЛЫЙ ЭКРАН)
    engine.rootContext()->setContextProperty("dbManager", &DatabaseManager::instance());
    engine.rootContext()->setContextProperty("myRecipeService", &service);
    engine.rootContext()->setContextProperty("mainModel", &mainModel);
    engine.rootContext()->setContextProperty("pantryModel", &pantryModel);
    engine.rootContext()->setContextProperty("dbManager", &DatabaseManager::instance());
    engine.rootContext()->setContextProperty("myRecipeService", &service);

    const QUrl url(u"qrc:/RecipeWizard/qml/main.qml"_qs);
    engine.load(url);

    return app.exec();
}