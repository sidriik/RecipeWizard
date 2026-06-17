#pragma once

#include <QObject> // 1. Обязательно добавь этот инклюд
#include <QList>
#include <QString>
#include <QStringList>
#include "Recipe.h"
#include "RecipeRepository.h"

// 2. Добавь наследование от QObject
class RecipeService : public QObject
{
    Q_OBJECT // 3. Добавь этот макрос первым делом в теле класса

public:
    // 4. Обнови конструктор, чтобы он принимал parent
    explicit RecipeService(RecipeRepository* repository, QObject* parent = nullptr);

    QList<Recipe> getAllRecipes();

    bool addRecipe(const Recipe& recipe);
    bool updateRecipe(const Recipe& recipe);
    bool deleteRecipe(int id);
    Recipe getRecipeById(int id);
    bool recipeExists(const QString& title);
    QList<Recipe> searchRecipes(const QString& ingredientsText);
    QList<Recipe> findBestRecipes(const QStringList& availableIngredients);
    QList<Recipe> filterByTags(const QStringList& tags);

    // 5. Оставь Q_INVOKABLE для тех функций, которые вызываешь из QML кода
    Q_INVOKABLE QString getAIAnalysis(const QString &title, const QString &tags, int calories);
    Q_INVOKABLE double calculateDailyCalories(double weight, double height, int age, bool isMale, double activityFactor);
    Q_INVOKABLE QString scaleIngredients(const QString &ingredients, double multiplier);
    QList<Recipe> searchByTitle(const QString& titleText);
    // Метод для получения чистого названия продукта без веса/количества
    Q_INVOKABLE QString getCleanName(const QString &ingredient);
    Q_INVOKABLE QStringList getStepList(const QString &instructions);
    // Найти количество секунд для таймера (например, из "варить 5 минут")
    Q_INVOKABLE int getTimerSeconds(const QString &stepText);

    struct Nutrients {
        double calories;
        double proteins;
        double fats;
        double carbs;
    };

    // Сделаем метод, возвращающий строку с данными через разделитель, чтобы QML легко её распарсил
    Q_INVOKABLE QString calculateFullNutrients(double weight, double height, int age, bool isMale, double activity);

private:
    RecipeRepository* repository_;
};