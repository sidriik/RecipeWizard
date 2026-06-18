#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>

/**
 * @brief Класс для работы с базой данных приложения.
 */
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Доступ к единственному экземпляру.
     * @return Ссылка на одиночку DatabaseManager.
     */
    static DatabaseManager &instance();

    /**
     * @brief Открыть базу и подготовить данные.
     * @param databasePath Путь к файлу базы (по умолчанию "recipes.db").
     * @param imageBase Базовый префикс для путей к картинкам рецептов.
     */
    void init(const QString &databasePath = QStringLiteral("recipes.db"),
              const QString &imageBase =
              QStringLiteral("qrc:/RecipeWizard/qml/images/"));

    /**
     * @brief Текущее соединение с базой.
     * @return Объект QSqlDatabase, используемый приложением.
     */
    QSqlDatabase database() const { return m_db; }

    /**
     * @brief Добавить рецепт в избранное.
     * @param title Название блюда.
     * @return true при успехе.
     */
    Q_INVOKABLE bool addFavorite(const QString &title);

    /**
     * @brief Убрать рецепт из избранного.
     * @param title Название блюда.
     * @return true при успехе.
     */
    Q_INVOKABLE bool removeFavorite(const QString &title);

    /**
     * @brief Проверить, находится ли рецепт в избранном.
     * @param title Название блюда.
     * @return true, если рецепт в избранном.
     */
    Q_INVOKABLE bool isFavorite(const QString &title);

    /**
     * @brief Получить список избранных рецептов.
     * @return Названия избранных блюд по алфавиту.
     */
    Q_INVOKABLE QStringList getFavorites();

    /**
     * @brief Переключить статус избранного.
     * @param title Название блюда.
     * @return Новое состояние: true — теперь в избранном, false — нет.
     */
    Q_INVOKABLE bool toggleFavorite(const QString &title);

    /**
     * @brief Записать просмотр рецепта в историю.
     * @param title Название блюда.
     */
    Q_INVOKABLE void addToHistory(const QString &title);

    /**
     * @brief Получить историю просмотров без повторов.
     * @return Названия блюд от новых к старым.
     */
    Q_INVOKABLE QStringList getHistory();

    /**
     * @brief Добавить рецепт в таблицу рецептов.
     * @param title Название блюда.
     * @param ingredients Ингредиенты строкой через запятую.
     * @param tags Теги строкой через запятую.
     * @param calories Калорийность порции.
     * @param proteins Белки, г.
     * @param fats Жиры, г.
     * @param carbs Углеводы, г.
     * @param servings Число порций.
     * @param instructions Пошаговая инструкция.
     * @return true, если рецепт добавлен; false, если он уже существует.
     */
    Q_INVOKABLE bool addRecipe(const QString &title, const QString &ingredients,
                               const QString &tags, int calories, double proteins,
                               double fats, double carbs, int servings,
                               const QString &instructions);

    /**
     * @brief Проверить наличие рецепта по названию.
     * @param title Название блюда.
     * @return true, если рецепт существует.
     */
    Q_INVOKABLE bool recipeExists(const QString &title);

    /**
     * @brief Удалить рецепт по идентификатору.
     * @param id Идентификатор записи.
     * @return true при успехе.
     */
    Q_INVOKABLE bool deleteRecipe(int id);

    /**
     * @brief Добавить продукты в список покупок.
     * @param ingredients Продукты строкой через запятую.
     */
    Q_INVOKABLE void addToShoppingList(const QString &ingredients);

    /**
     * @brief Получить список покупок.
     * @return Названия продуктов в списке покупок.
     */
    Q_INVOKABLE QStringList getShoppingList();

    /**
     * @brief Удалить продукт из списка покупок.
     * @param name Название продукта.
     */
    Q_INVOKABLE void removeFromShoppingList(const QString &name);

    /**
     * @brief Очистить список покупок.
     */
    Q_INVOKABLE void clearShoppingList();

    /**
     * @brief Добавить продукт в холодильник.
     * @param name Название продукта.
     */
    Q_INVOKABLE void addToFridge(const QString &name);

    /**
     * @brief Получить содержимое холодильника.
     * @return Названия продуктов в холодильнике по алфавиту.
     */
    Q_INVOKABLE QStringList getFridgeList();

    /**
     * @brief Удалить продукт из холодильника.
     * @param name Название продукта.
     */
    Q_INVOKABLE void removeFromFridge(const QString &name);

    /**
     * @brief Очистить холодильник.
     */
    Q_INVOKABLE void clearFridge();

signals:
    /// Содержимое холодильника изменилось.
    void fridgeUpdated();
    /// Список покупок изменился.
    void shoppingListUpdated();

private:
    explicit DatabaseManager(QObject *parent = nullptr);

    void createTables();
    void seedData();

    QSqlDatabase m_db;
    QString m_imageBase;
};
