#include "RecipeModel.h"

#include <algorithm>

#include "DatabaseManager.h"
#include "RecipeException.h"

RecipeModel::RecipeModel(RecipeService *service, QObject *parent)
    : QAbstractListModel(parent), service_(service)
{
    refresh();
}

int RecipeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return recipes_.size();
}

QVariant RecipeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= recipes_.size())
        return {};

    const Recipe &recipe = recipes_.at(index.row());
    switch (role) {
    case IdRole: return recipe.id;
    case TitleRole: return recipe.title;
    case ImageUrlRole: return recipe.imageUrl;
    case IngredientsRole: return recipe.ingredients;
    case TagsRole: return recipe.tags;
    case CaloriesRole: return recipe.calories;
    case ProteinsRole: return recipe.proteins;
    case FatsRole: return recipe.fats;
    case CarbsRole: return recipe.carbs;
    case ServingsRole: return recipe.servings;
    case InstructionsRole: return recipe.instructions;
    default: return {};
    }
}

QHash<int, QByteArray> RecipeModel::roleNames() const
{
    return {
            {IdRole, "id"},
            {TitleRole, "title"},
            {ImageUrlRole, "imageUrl"},
            {IngredientsRole, "ingredients"},
            {TagsRole, "tags"},
            {CaloriesRole, "calories"},
            {ProteinsRole, "proteins"},
            {FatsRole, "fats"},
            {CarbsRole, "carbs"},
            {ServingsRole, "servings"},
            {InstructionsRole, "instructions"}};
}

void RecipeModel::setRecipes(QList<Recipe> recipes)
{
    beginResetModel();
    recipes_ = std::move(recipes);
    endResetModel();
}

void RecipeModel::refresh()
{
    try {
        setRecipes(service_->getAllRecipes());
    } catch (const RecipeException &error) {
        qWarning("Ошибка загрузки рецептов: %s", error.what());
        setRecipes({});
    }
}

void RecipeModel::findBestRecipes(const QString &ingredientsText)
{
    try {
        const QStringList selected = ingredientsText.split(QLatin1Char(','), Qt::SkipEmptyParts);
        setRecipes(service_->findBestRecipes(selected));
    } catch (const RecipeException &error) {
        qWarning("Ошибка подбора рецептов: %s", error.what());
        setRecipes({});
    }
}

void RecipeModel::applyFilters(const QStringList &tags, int maxCal)
{
    activeTags_ = tags;
    maxCalories_ = maxCal;
    try {
        QList<Recipe> filtered;
        for (const Recipe &recipe : service_->getAllRecipes()) {
            bool caloriesMatch = true;
            if (maxCalories_ == 300)
                caloriesMatch = recipe.calories <= 300;
            else if (maxCalories_ == 600)
                caloriesMatch = recipe.calories > 300 && recipe.calories <= 600;

            bool tagsMatch = true;
            const QString recipeTags = recipe.tags.toLower();
            for (const QString &tag : activeTags_) {
                if (!recipeTags.contains(tag.trimmed().toLower())) {
                    tagsMatch = false;
                    break;
                }
            }
            if (caloriesMatch && tagsMatch)
                filtered.append(recipe);
        }
        setRecipes(filtered);
        if (currentSort_ != "Без сортировки")
            sortRecipes(currentSort_);
    } catch (const RecipeException &error) {
        qWarning("Ошибка фильтрации: %s", error.what());
        setRecipes({});
    }
}

void RecipeModel::searchByTitle(const QString &text)
{
    try {
        setRecipes(service_->searchByTitle(text));
    } catch (const RecipeException &error) {
        qWarning("Ошибка поиска по названию: %s", error.what());
        setRecipes({});
    }
}

void RecipeModel::sortRecipes(const QString &sortType)
{
    currentSort_ = sortType;
    beginResetModel();
    std::sort(recipes_.begin(), recipes_.end(), [&](const Recipe &a, const Recipe &b) {
        if (sortType == QStringLiteral("Калории ↑")) return a.calories < b.calories;
        if (sortType == QStringLiteral("Калории ↓")) return a.calories > b.calories;
        if (sortType == QStringLiteral("Белки ↑")) return a.proteins < b.proteins;
        if (sortType == QStringLiteral("Белки ↓")) return a.proteins > b.proteins;
        if (sortType == QStringLiteral("Жиры ↑")) return a.fats < b.fats;
        if (sortType == QStringLiteral("Жиры ↓")) return a.fats > b.fats;
        if (sortType == QStringLiteral("Углеводы ↑")) return a.carbs < b.carbs;
        if (sortType == QStringLiteral("Углеводы ↓")) return a.carbs > b.carbs;
        return false;
    });
    endResetModel();
}

void RecipeModel::toggleFavoriteInModel(int row)
{
    if (row < 0 || row >= recipes_.size())
        return;
    DatabaseManager::instance().toggleFavorite(recipes_.at(row).title);
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant RecipeModel::getVal(int row, const QString &roleName)
{
    if (row < 0 || row >= recipes_.size())
        return {};

    const QByteArray target = roleName.toUtf8();
    const QHash<int, QByteArray> roles = roleNames();
    for (auto it = roles.cbegin(); it != roles.cend(); ++it) {
        if (it.value() == target)
            return data(index(row, 0), it.key());
    }
    return {};
}


