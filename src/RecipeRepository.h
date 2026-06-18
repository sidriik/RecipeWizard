#pragma once

#include <QList>
#include <QSqlDatabase>
#include <QString>

#include "Recipe.h"

/**
 * @brief Работа с таблицей рецептов в SQLite.
 */
class RecipeRepository
{
public:
    /**
     * @brief Создать репозиторий поверх открытого соединения с базой.
     * @param db Соединение с базой данных.
     */
    explicit RecipeRepository(QSqlDatabase db);

    /**
     * @brief Прочитать все рецепты.
     * @return Список рецептов.
     */
    QList<Recipe> getAllRecipes();

    /**
     * @brief Добавить новый рецепт.
     * @param recipe Рецепт для добавления.
     * @return true, если запись добавлена.
     */
    bool addRecipe(const Recipe &recipe);

    /**
     * @brief Получить рецепт по идентификатору.
     * @param id Идентификатор записи.
     * @return Найденный рецепт; если записи нет — рецепт с id == -1.
     */
    Recipe getRecipeById(int id);


    /**
     * @brief Проверить, существует ли рецепт с таким названием.
     * @param title Название блюда.
     * @return true, если рецепт с таким названием уже есть.
     */
    bool recipeExists(const QString &title);

private:
    QSqlDatabase db_;
};
