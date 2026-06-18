#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "Recipe.h"
#include "RecipeRepository.h"
#include "RecipeService.h"

namespace {

QSqlDatabase makeMemoryDatabase(const QString &name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), name);
    db.setDatabaseName(QStringLiteral(":memory:"));
    REQUIRE(db.open());
    QSqlQuery query(db);
    REQUIRE(query.exec(QStringLiteral(
        "CREATE TABLE recipes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL, ingredients TEXT NOT NULL, tags TEXT,"
        "calories INTEGER, proteins REAL, fats REAL, carbs REAL,"
        "servings INTEGER, instructions TEXT, imageUrl TEXT)")));
    return db;
}

Recipe sampleRecipe(const QString &title, const QString &ingredients,
                    const QString &tags, int calories)
{
    Recipe recipe;
    recipe.title = title;
    recipe.ingredients = ingredients;
    recipe.tags = tags;
    recipe.calories = calories;
    recipe.servings = 2;
    return recipe;
}

} // namespace

TEST_CASE("scaleIngredients масштабирует и не трогает строку при множителе 1")
{
    RecipeService service(nullptr);
    CHECK(service.scaleIngredients(QStringLiteral("2 яйца, 50 сыр"), 2.0)
          == QStringLiteral("4 яйца, 100 сыр"));
    CHECK(service.scaleIngredients(QStringLiteral("2 яйца"), 1.0)
          == QStringLiteral("2 яйца"));
}

TEST_CASE("getCleanName убирает количества и единицы измерения")
{
    RecipeService service(nullptr);
    CHECK(service.getCleanName(QStringLiteral("50г Сыр Чеддер"))
          == QStringLiteral("Сыр Чеддер"));
    CHECK(service.getCleanName(QStringLiteral("   ")).isEmpty());
}

TEST_CASE("getTimerSeconds находит минуты и обрабатывает их отсутствие")
{
    RecipeService service(nullptr);
    CHECK(service.getTimerSeconds(QStringLiteral("варить 5 минут")) == 300);
    CHECK(service.getTimerSeconds(QStringLiteral("посолить по вкусу")) == 0);
}

TEST_CASE("getStepList разбивает инструкцию на шаги")
{
    RecipeService service(nullptr);
    CHECK(service.getStepList(QStringLiteral("Шаг 1\nШаг 2\nШаг 3")).size() == 3);
    CHECK(service.getStepList(QString()).isEmpty());
}

TEST_CASE("calculateFullNutrients возвращает четыре значения через разделитель")
{
    RecipeService service(nullptr);
    const QStringList parts =
        service.calculateFullNutrients(70, 175, 25, true, 1.2).split(QLatin1Char('|'));
    // Положительный случай: ровно четыре части и калории положительны.
    REQUIRE(parts.size() == 4);
    CHECK(parts.at(0).toDouble() > 0.0);
    // Отрицательный случай: при нулевых параметрах результат всё равно структурно корректен.
    CHECK(service.calculateFullNutrients(0, 0, 0, false, 0).split(QLatin1Char('|')).size() == 4);
}

TEST_CASE("Репозиторий добавляет рецепты и проверяет их существование")
{
    {
        QSqlDatabase db = makeMemoryDatabase(QStringLiteral("repo_exists"));
        RecipeRepository repository(db);

        CHECK(repository.addRecipe(
            sampleRecipe(QStringLiteral("Омлет"),
                         QStringLiteral("яйца, молоко"),
                         QStringLiteral("завтрак"),
                         320)));

        CHECK(repository.recipeExists(QStringLiteral("Омлет")));
        CHECK_FALSE(repository.recipeExists(QStringLiteral("Борщ")));
        CHECK(repository.getAllRecipes().size() == 1);
    }
    QSqlDatabase::removeDatabase(QStringLiteral("repo_exists"));
}

TEST_CASE("findBestRecipes ранжирует рецепты по числу совпадений")
{
    {
        QSqlDatabase db = makeMemoryDatabase(QStringLiteral("best"));
        RecipeRepository repository(db);
        repository.addRecipe(sampleRecipe(QStringLiteral("Яичница"),
                                          QStringLiteral("яйца, масло"),
                                          QStringLiteral("завтрак"), 200));
        repository.addRecipe(sampleRecipe(QStringLiteral("Омлет"),
                                          QStringLiteral("яйца, молоко, сыр"),
                                          QStringLiteral("завтрак"), 320));

        RecipeService service(&repository);
        const QList<Recipe> best = service.findBestRecipes(
            {QStringLiteral("яйца"), QStringLiteral("молоко"), QStringLiteral("сыр")});
        // Положительный случай: первым идёт рецепт с большим числом совпадений.
        REQUIRE(best.size() == 2);
        CHECK(best.first().title == QStringLiteral("Омлет"));
        // Отрицательный случай: продукт, которого нет ни в одном рецепте, ничего не находит.
        CHECK(service.findBestRecipes({QStringLiteral("ананас")}).isEmpty());
    }
    QSqlDatabase::removeDatabase(QStringLiteral("best"));
}

TEST_CASE("filterByTags и searchByTitle отбирают нужные рецепты")
{
    {
        QSqlDatabase db = makeMemoryDatabase(QStringLiteral("filter"));
        RecipeRepository repository(db);
        repository.addRecipe(sampleRecipe(QStringLiteral("Веганский суп"),
                                          QStringLiteral("овощи"),
                                          QStringLiteral("обед, веган"), 180));
        repository.addRecipe(sampleRecipe(QStringLiteral("Стейк"),
                                          QStringLiteral("говядина"),
                                          QStringLiteral("ужин, кето"), 750));

        RecipeService service(&repository);
        // Положительный случай: по тегу "веган" находится один рецепт.
        CHECK(service.filterByTags({QStringLiteral("веган")}).size() == 1);
        // Отрицательный случай: несуществующий тег ничего не возвращает.
        CHECK(service.filterByTags({QStringLiteral("фастфуд")}).isEmpty());
        // Поиск по названию: положительный и отрицательный случаи.
        CHECK(service.searchByTitle(QStringLiteral("стейк")).size() == 1);
        CHECK(service.searchByTitle(QStringLiteral("пицца")).isEmpty());

    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
