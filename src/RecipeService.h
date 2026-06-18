#pragma once

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include "Recipe.h"
#include "RecipeRepository.h"

/**
 * @brief Сервис для работы с рецептами.
 */
class RecipeService : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Создать сервис.
     * @param repository Репозиторий рецептов.
     * @param parent Родитель в дереве объектов Qt.
     */
    explicit RecipeService(RecipeRepository *repository, QObject *parent = nullptr);

    /**
     * @brief Получить все рецепты.
     * @return Полный список рецептов из базы.
     */
    QList<Recipe> getAllRecipes();

    /**
     * @brief Добавить рецепт.
     * @param recipe Заполненная структура рецепта.
     * @return true при успехе.
     */
    bool addRecipe(const Recipe &recipe);

    /**
     * @brief Получить рецепт по идентификатору.
     * @param id Идентификатор записи.
     * @return Найденный рецепт либо рецепт с id == -1.
     */
    Recipe getRecipeById(int id);

    /**
     * @brief Проверить наличие рецепта с заданным названием.
     * @param title Название блюда.
     * @return true, если такой рецепт уже есть.
     */
    bool recipeExists(const QString &title);

    /**
     * @brief Найти рецепты, в составе которых встречается хотя бы один продукт.
     * @param ingredientsText Продукты одной строкой через запятую.
     * @return Подходящие рецепты; при пустом запросе — все рецепты.
     */
    QList<Recipe> searchRecipes(const QString &ingredientsText);

    /**
     * @brief Подобрать рецепты по имеющимся продуктам.
      * @param availableIngredients Список продуктов.
      * @return Список подходящих рецептов.
      */
    QList<Recipe> findBestRecipes(const QStringList &availableIngredients);

    /**
     * @brief Отфильтровать рецепты, содержащие все указанные теги.
     * @param tags Список тегов, которые должны присутствовать одновременно.
     * @return Рецепты, удовлетворяющие всем тегам.
     */
    QList<Recipe> filterByTags(const QStringList &tags);

    /**
     * @brief Найти рецепты по подстроке в названии.
     * @param titleText Искомая подстрока; при пустой строке — все рецепты.
     * @return Рецепты, в названии которых встречается подстрока.
     */
    QList<Recipe> searchByTitle(const QString &titleText);

    /**
     * @brief Сформировать рекомендацию по рецепту.
     */
    Q_INVOKABLE QString getRecipeAdvice(const QString &title, const QString &tags,
                                      int calories);

    /**
     * @brief Рассчитать суточную норму калорий по формуле Миффлина — Сан Жеора.
     * @param weight Вес, кг.
     * @param height Рост, см.
     * @param age Возраст, лет.
     * @param isMale true для мужчины, false для женщины.
     * @param activityFactor Коэффициент активности.
     * @return Суточная норма калорий, ккал.
     */
    Q_INVOKABLE double calculateDailyCalories(double weight, double height, int age,
                                              bool isMale, double activityFactor);

    /**
     * @brief Пересчитать числа в строке ингредиентов под другое число порций.
     * @param ingredients Исходная строка ингредиентов.
     * @param multiplier Множитель (отношение нужных порций к исходным).
     * @return Строка с пересчитанными количествами.
     */
    Q_INVOKABLE QString scaleIngredients(const QString &ingredients, double multiplier);

    /**
     * @brief Очистить название ингредиента от количеств и единиц измерения.
     * @param ingredient Сырой пункт списка (например, "100г Сыр Чеддер").
     * @return Чистое название продукта (например, "Сыр Чеддер").
     */
    Q_INVOKABLE QString getCleanName(const QString &ingredient);

    /**
     * @brief Разбить инструкцию на отдельные шаги.
     * @param instructions Текст инструкции, шаги разделены переводом строки.
     * @return Список непустых шагов.
     */
    Q_INVOKABLE QStringList getStepList(const QString &instructions);

    /**
     * @brief Определить длительность таймера по тексту шага.
     * @param stepText Текст шага (например, "варить 5 минут").
     * @return Время в секундах либо 0, если время в шаге не указано.
     */
    Q_INVOKABLE int getTimerSeconds(const QString &stepText);

    /**
     * @brief Рассчитать норму калорий и БЖУ одной строкой для QML.
     * @param weight Вес, кг.
     * @param height Рост, см.
     * @param age Возраст, лет.
     * @param isMale true для мужчины, false для женщины.
     * @param activity Коэффициент активности.
     * @return Строка вида "калории|белки|жиры|углеводы".
     */
    Q_INVOKABLE QString calculateFullNutrients(double weight, double height, int age,
                                               bool isMale, double activity);

private:
    RecipeRepository *repository_; ///< Источник данных о рецептах.
};
