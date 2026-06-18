#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <QVariant>

#include "Recipe.h"
#include "RecipeService.h"

/**
 * @brief Модель списка рецептов для отображения в QML.
 */
class RecipeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /// Роли, под которыми поля рецепта доступны из QML.
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ImageUrlRole,
        IngredientsRole,
        TagsRole,
        CaloriesRole,
        ProteinsRole,
        FatsRole,
        CarbsRole,
        ServingsRole,
        InstructionsRole
    };

    /**
     * @brief Создать модель и сразу загрузить все рецепты.
     * @param service Сервис-источник данных.
     * @param parent Родитель в дереве объектов Qt.
     */
    explicit RecipeModel(RecipeService *service, QObject *parent = nullptr);

    /**
     * @brief Число строк в модели.
     * @param parent Родительский индекс.
     * @return Количество рецептов.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Значение поля рецепта по индексу и роли.
     * @param index Индекс строки.
     * @param role Запрашиваемая роль из перечисления Roles.
     * @return Значение поля либо недействительный QVariant.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Имена ролей модели.
     * @return Список ролей.
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Перезагрузить полный список рецептов.
     */
    Q_INVOKABLE void refresh();

    /**
     * @brief Подобрать рецепты под имеющиеся продукты.
     * @param ingredientsText Продукты строкой через запятую.
     */
    Q_INVOKABLE void findBestRecipes(const QString &ingredientsText);

    /**
     * @brief Отфильтровать рецепты по тегам и максимуму калорий.
     * @param tags Выбранные теги.
     * @param maxCal Граница калорийности: 300 — «легко», 600 — «сытно», иначе без ограничения.
     */
    Q_INVOKABLE void applyFilters(const QStringList &tags, int maxCal);

    /**
     * @brief Найти рецепты по подстроке в названии.
     * @param text Искомая подстрока.
     */
    Q_INVOKABLE void searchByTitle(const QString &text);

    /**
     * @brief Отсортировать текущий список рецептов.
     * @param sortType Тип сортировки.
     */
    Q_INVOKABLE void sortRecipes(const QString &sortType);

    /**
     * @brief Переключить избранное для рецепта в указанной строке.
     * @param row Номер строки в модели.
     */
    Q_INVOKABLE void toggleFavoriteInModel(int row);

    /**
     * @brief Получить значение поля строки по имени роли.
     * @param row Номер строки.
     * @param roleName Имя роли.
     * @return Значение поля либо недействительный QVariant.
     */
    Q_INVOKABLE QVariant getVal(int row, const QString &roleName);

private:
    void setRecipes(QList<Recipe> recipes);

    RecipeService *service_;  ///< Источник данных.
    QList<Recipe> recipes_;   ///< Текущий показываемый список.
    QStringList activeTags_;  ///< Выбранные теги фильтра.
    int maxCalories_ = 2000;  ///< Текущая граница калорийности.
};
