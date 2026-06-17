#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QStringList>

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    static DatabaseManager& instance();
    void init();
    Q_INVOKABLE bool addFavorite(const QString &title);
    Q_INVOKABLE bool removeFavorite(const QString &title);
    Q_INVOKABLE bool isFavorite(const QString &title);
    Q_INVOKABLE QStringList getFavorites();
    Q_INVOKABLE void addToHistory(const QString &title);
    Q_INVOKABLE QStringList getHistory();
    Q_INVOKABLE bool addRecipe(
        const QString &title,
        const QString &ingredients,
        const QString &tags,
        int calories,
        double proteins,
        double fats,
        double carbs,
        int servings,
        const QString &instructions
        );
    Q_INVOKABLE bool recipeExists(const QString &title);
    Q_INVOKABLE bool deleteRecipe(int id);
    Q_INVOKABLE bool updateRecipe(int id, const QString &title, const QString &ingredients, const QString &tags,
                                  int calories, double proteins, double fats, double carbs,
                                  int servings, const QString &instructions);
    Q_INVOKABLE bool registerUser(
        const QString &username,
        const QString &password
        );

    Q_INVOKABLE bool loginUser(
        const QString &username,
        const QString &password
        );
    Q_INVOKABLE QStringList getCategories();
    Q_INVOKABLE QStringList getAllTags();
    Q_INVOKABLE bool toggleFavorite(const QString &title);
    Q_INVOKABLE void addToShoppingList(const QString &ingredients);
    Q_INVOKABLE QStringList getShoppingList();
    Q_INVOKABLE void removeFromShoppingList(const QString &name);
    Q_INVOKABLE void clearShoppingList();
    Q_INVOKABLE void addToFridge(const QString &name);
    Q_INVOKABLE QStringList getFridgeList();
    Q_INVOKABLE void removeFromFridge(const QString &name);
    Q_INVOKABLE void clearFridge();
    QSqlDatabase database() const { return m_db; }
signals:
    void fridgeUpdated();
    void shoppingListUpdated();
    void favoritesChanged();

private:
    QSqlDatabase m_db;
    explicit DatabaseManager(QObject *parent = nullptr);
    QSqlDatabase db;
    void createTables();
    void seedData();
};