#include "RecipeRepository.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

RecipeRepository::RecipeRepository(QSqlDatabase db)
    : db_(db)
{
}

QList<Recipe> RecipeRepository::getAllRecipes()
{
    QList<Recipe> recipes;
    // Читаем все 11 колонок
    QSqlQuery query("SELECT id, title, ingredients, tags, calories, proteins, fats, carbs, servings, instructions, imageUrl FROM recipes", db_);

    if (!query.exec()) {
        qWarning() << "ОШИБКА SELECT getAllRecipes:" << query.lastError().text();
        return recipes;
    }

    while (query.next()) {
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
        recipes.append(recipe);
    }
    return recipes;
}

bool RecipeRepository::addRecipe(const Recipe& recipe)
{
    QSqlQuery query(db_);
    query.prepare(
        "INSERT INTO recipes "
        "(title, ingredients, tags, calories, proteins, fats, carbs, servings, instructions, imageUrl) "
        "VALUES "
        "(:title, :ingredients, :tags, :calories, :proteins, :fats, :carbs, :servings, :instructions, :imageUrl)"
        );

    query.bindValue(":title", recipe.title);
    query.bindValue(":ingredients", recipe.ingredients);
    query.bindValue(":tags", recipe.tags);
    query.bindValue(":calories", recipe.calories);
    query.bindValue(":proteins", recipe.proteins);
    query.bindValue(":fats", recipe.fats);
    query.bindValue(":carbs", recipe.carbs);
    query.bindValue(":servings", recipe.servings);
    query.bindValue(":instructions", recipe.instructions);
    query.bindValue(":imageUrl", recipe.imageUrl);

    return query.exec();
}

bool RecipeRepository::deleteRecipe(int id)
{
    QSqlQuery query(db_);
    query.prepare("DELETE FROM recipes WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

Recipe RecipeRepository::getRecipeById(int id)
{
    Recipe recipe;
    QSqlQuery query(db_);
    // ИСПРАВЛЕНО: Добавлен imageUrl в выборку
    query.prepare("SELECT id, title, ingredients, tags, calories, proteins, fats, carbs, servings, instructions, imageUrl "
                  "FROM recipes WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
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
    }
    return recipe;
}

bool RecipeRepository::updateRecipe(const Recipe& recipe)
{
    QSqlQuery query(db_);
    // ИСПРАВЛЕНО: Добавлена запятая после :imageUrl
    query.prepare(
        "UPDATE recipes SET "
        "title = :title, "
        "imageUrl = :imageUrl, "
        "ingredients = :ingredients, "
        "tags = :tags, "
        "calories = :calories, "
        "proteins = :proteins, "
        "fats = :fats, "
        "carbs = :carbs, "
        "servings = :servings, "
        "instructions = :instructions "
        "WHERE id = :id"
        );

    query.bindValue(":id", recipe.id);
    query.bindValue(":title", recipe.title);
    query.bindValue(":imageUrl", recipe.imageUrl);
    query.bindValue(":ingredients", recipe.ingredients);
    query.bindValue(":tags", recipe.tags);
    query.bindValue(":calories", recipe.calories);
    query.bindValue(":proteins", recipe.proteins);
    query.bindValue(":fats", recipe.fats);
    query.bindValue(":carbs", recipe.carbs);
    query.bindValue(":servings", recipe.servings);
    query.bindValue(":instructions", recipe.instructions);

    return query.exec();
}

bool RecipeRepository::recipeExists(const QString& title)
{
    QSqlQuery query(db_);
    query.prepare("SELECT COUNT(*) FROM recipes WHERE title = :title");
    query.bindValue(":title", title);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}