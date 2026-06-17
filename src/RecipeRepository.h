#pragma once

#include <QList>
#include <QSqlDatabase>

#include "Recipe.h"

class RecipeRepository
{
public:
    explicit RecipeRepository(QSqlDatabase db);

    QList<Recipe> getAllRecipes();
    bool addRecipe(const Recipe& recipe);

    Recipe getRecipeById(int id);
    bool updateRecipe(const Recipe& recipe);
    bool deleteRecipe(int id);
    bool recipeExists(const QString& title);

private:
    QSqlDatabase db_;
};