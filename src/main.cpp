#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSqlDatabase>
#include <QDebug>

#include "DatabaseManager.h"
#include "RecipeException.h"
#include "RecipeModel.h"
#include "RecipeService.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("RecipeWizard"));
    QGuiApplication::setApplicationVersion(QStringLiteral("1.0"));
    QQuickStyle::setStyle(QStringLiteral("Basic"));
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Приложение с рецептами RecipeWizard"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption dbOption({QStringLiteral("d"), QStringLiteral("db")},
                                QStringLiteral("Путь к файлу базы данных."),
                                QStringLiteral("path"),
                                QStringLiteral("recipes.db"));
    QCommandLineOption imagesOption({QStringLiteral("i"), QStringLiteral("images")},
                                    QStringLiteral("Путь к папке с картинками рецептов."),
                                    QStringLiteral("dir"));
    parser.addOption(dbOption);
    parser.addOption(imagesOption);
    parser.process(app);

    QString imageBase = "qrc:/qt/qml/RecipeWizard/qml/images/";
    if (parser.isSet(imagesOption)) {
        QString dir = parser.value(imagesOption);
        dir.replace(QLatin1Char('\\'), QLatin1Char('/'));
        if (!dir.endsWith(QLatin1Char('/')))
            dir.append(QLatin1Char('/'));
        imageBase = QStringLiteral("file:///") + dir;
    }

    try {
        DatabaseManager::instance().init(parser.value(dbOption), imageBase);
    } catch (const RecipeException &error) {
        qCritical("Не удалось открыть базу данных: %s", error.what());
        return 1;
    }

    RecipeRepository repository(QSqlDatabase::database());
    RecipeService service(&repository);
    RecipeModel mainModel(&service);
    RecipeModel pantryModel(&service);

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty(QStringLiteral("dbManager"), &DatabaseManager::instance());
    context->setContextProperty(QStringLiteral("myRecipeService"), &service);
    context->setContextProperty(QStringLiteral("mainModel"), &mainModel);
    context->setContextProperty(QStringLiteral("pantryModel"), &pantryModel);

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(1); }, Qt::QueuedConnection);

    engine.loadFromModule("RecipeWizard", "Main");
    return app.exec();
}
