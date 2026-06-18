#include "RecipeRepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include "RecipeException.h"

namespace {

const QString kSelectColumns =
    QStringLiteral("id, title, ingredients, tags, calories, proteins, fats, "
                   "carbs, servings, instructions, imageUrl");

// Выполнить запрос или бросить исключение с текстом ошибки SQLite.
void execOrThrow(QSqlQuery &query)
{
    if (!query.exec())
        throw RecipeException(query.lastError().text());
}

// Прочитать одну строку результата в структуру рецепта.
Recipe readRow(const QSqlQuery &query)
{
    Recipe recipe;
    recipe.id = query.value(0).toInt();
    recipe.title = query.value(1).toString();
    recipe.ingredients = query.value(2).toString();
    recipe.tags = query.value(3).toString();
    recipe.calories = query.value(4).toInt();
    recipe.proteins = query.value(5).toDouble();
    recipe.fats = query.value(6).toDouble();
    recipe.carbs = query.value(7).toDouble();
    recipe.servings = query.value(8).toInt();
    recipe.instructions = query.value(9).toString();
    recipe.imageUrl = query.value(10).toString();
    return recipe;
}

} // namespace

RecipeRepository::RecipeRepository(QSqlDatabase db)
    : db_(db)
{
}

QList<Recipe> RecipeRepository::getAllRecipes()
{
    QList<Recipe> recipes;
    QSqlQuery query(QStringLiteral("SELECT %1 FROM recipes").arg(kSelectColumns), db_);
    execOrThrow(query);
    while (query.next())
        recipes.append(readRow(query));
    return recipes;
}

bool RecipeRepository::addRecipe(const Recipe &recipe)
{
    QSqlQuery query(db_);
    query.prepare(QStringLiteral(
        "INSERT INTO recipes "
        "(title, ingredients, tags, calories, proteins, fats, carbs, "
        "servings, instructions, imageUrl) "
        "VALUES (:title, :ingredients, :tags, :calories, :proteins, :fats, "
        ":carbs, :servings, :instructions, :imageUrl)"));
    query.bindValue(QStringLiteral(":title"), recipe.title);
    query.bindValue(QStringLiteral(":ingredients"), recipe.ingredients);
    query.bindValue(QStringLiteral(":tags"), recipe.tags);
    query.bindValue(QStringLiteral(":calories"), recipe.calories);
    query.bindValue(QStringLiteral(":proteins"), recipe.proteins);
    query.bindValue(QStringLiteral(":fats"), recipe.fats);
    query.bindValue(QStringLiteral(":carbs"), recipe.carbs);
    query.bindValue(QStringLiteral(":servings"), recipe.servings);
    query.bindValue(QStringLiteral(":instructions"), recipe.instructions);
    query.bindValue(QStringLiteral(":imageUrl"), recipe.imageUrl);
    execOrThrow(query);
    return true;
}

Recipe RecipeRepository::getRecipeById(int id)
{
    Recipe recipe;
    QSqlQuery query(db_);
    query.prepare(
        QStringLiteral("SELECT %1 FROM recipes WHERE id = :id").arg(kSelectColumns));
    query.bindValue(QStringLiteral(":id"), id);
    execOrThrow(query);
    if (query.next())
        recipe = readRow(query);
    return recipe;
}


bool RecipeRepository::recipeExists(const QString &title)
{
    QSqlQuery query(db_);
    query.prepare(QStringLiteral("SELECT COUNT(*) FROM recipes WHERE title = :title"));
    query.bindValue(QStringLiteral(":title"), title);
    execOrThrow(query);
    return query.next() && query.value(0).toInt() > 0;
}
