#pragma once

#include <QAbstractListModel>
#include <QStringList>

#include "RecipeService.h"

class RecipeModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit RecipeModel(RecipeService* service,
                         QObject* parent = nullptr);

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
        InstructionsRole,
        FavoriteRole = Qt::UserRole + 12
    };

    Q_INVOKABLE void searchRecipes(const QString& ingredient);
    Q_INVOKABLE void filterByTag(const QString& tag);
    Q_INVOKABLE void sortRecipes(const QString& sortType);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void findBestRecipes(const QString& ingredientsText);

    int rowCount(
        const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(
        const QModelIndex& index,
        int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void filterByTags(const QStringList& tags);
    Q_INVOKABLE QVariant getVal(int row, const QString &roleName);
    Q_INVOKABLE int getRowByTitle(const QString &title);
    Q_INVOKABLE void searchByTitle(const QString& text);
    Q_INVOKABLE void applyFilters(const QStringList &tags, int maxCal);
    Q_INVOKABLE void toggleFavoriteInModel(int row);
    Q_INVOKABLE void filterByFavorites(const QStringList &titles);


private:
    RecipeService* service_;
    QList<Recipe> recipes_;
    QStringList m_activeTags; // Список выбранных тегов
    int m_maxCalories = 2000;
};