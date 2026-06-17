#include "RecipeModel.h"
#include <algorithm>
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

RecipeModel::RecipeModel(RecipeService* service, QObject* parent)
    : QAbstractListModel(parent), service_(service)
{
    // Мы убрали отсюда автоматический коннект к refresh(),
    // чтобы лайки не сбрасывали твои фильтры на главной.
    refresh();
}

int RecipeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return recipes_.size();
}

QVariant RecipeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= recipes_.size())
        return QVariant();

    const Recipe& recipe = recipes_.at(index.row());

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
    default: return QVariant();
    }
}

QHash<int, QByteArray> RecipeModel::roleNames() const
{
    return {
        { IdRole, "id" },
        { TitleRole, "title" },
        { ImageUrlRole, "imageUrl" },
        { IngredientsRole, "ingredients" },
        { TagsRole, "tags" },
        { CaloriesRole, "calories" },
        { ProteinsRole, "proteins" },
        { FatsRole, "fats" },
        { CarbsRole, "carbs" },
        { ServingsRole, "servings" },
        { InstructionsRole, "instructions" }
    };
}

// Загрузка абсолютно всех рецептов
void RecipeModel::refresh()
{
    beginResetModel();
    recipes_ = service_->getAllRecipes();
    endResetModel();
}

// Поиск по ингредиентам (старый метод)
void RecipeModel::searchRecipes(const QString& ingredientsText)
{
    beginResetModel();
    recipes_ = service_->searchRecipes(ingredientsText);
    endResetModel();
}

// Одиночный фильтр по тегу
void RecipeModel::filterByTag(const QString& tag) {
    beginResetModel();
    if (tag.isEmpty() || tag == "Все") {
        recipes_ = service_->getAllRecipes();
    } else {
        QList<Recipe> allRecipes = service_->getAllRecipes();
        recipes_.clear();
        for (const Recipe& recipe : allRecipes) {
            if (recipe.tags.contains(tag.trimmed(), Qt::CaseInsensitive)) {
                recipes_.append(recipe);
            }
        }
    }
    endResetModel();
}

// Сортировка КБЖУ
void RecipeModel::sortRecipes(const QString& sortType)
{
    beginResetModel();
    std::sort(recipes_.begin(), recipes_.end(), [&](const Recipe& a, const Recipe& b) {
        if (sortType == "Калории ↑") return a.calories < b.calories;
        if (sortType == "Калории ↓") return a.calories > b.calories;
        if (sortType == "Белки ↑") return a.proteins < b.proteins;
        if (sortType == "Белки ↓") return a.proteins > b.proteins;
        if (sortType == "Жиры ↑") return a.fats < b.fats;
        if (sortType == "Жиры ↓") return a.fats > b.fats;
        return false;
    });
    endResetModel();
}

// ПОИСК ПО ХОЛОДИЛЬНИКУ (с ранжированием совпадений)
void RecipeModel::findBestRecipes(const QString &searchIngredients) {
    QStringList selectedList = searchIngredients.toLower().split(",", Qt::SkipEmptyParts);
    for (QString &item : selectedList) item = item.trimmed();

    if (selectedList.isEmpty()) {
        refresh();
        return;
    }

    struct RankedRecipe { Recipe item; int matchCount = 0; };
    QList<RankedRecipe> rankedRecipes;

    QSqlQuery query("SELECT id, title, ingredients, tags, calories, proteins, fats, carbs, servings, instructions, imageUrl FROM recipes",
                    DatabaseManager::instance().database());

    while (query.next()) {
        QString recipeIngredients = query.value("ingredients").toString().toLower();
        int matches = 0;
        for (const QString &ingredient : selectedList) {
            if (recipeIngredients.contains(ingredient)) matches++;
        }
        if (matches > 0) {
            Recipe r;
            r.id = query.value("id").toInt();
            r.title = query.value("title").toString();
            r.ingredients = query.value("ingredients").toString();
            r.tags = query.value("tags").toString();
            r.calories = query.value("calories").toInt();
            r.proteins = query.value("proteins").toDouble();
            r.fats = query.value("fats").toDouble();
            r.carbs = query.value("carbs").toDouble();
            r.servings = query.value("servings").toInt();
            r.instructions = query.value("instructions").toString();
            r.imageUrl = query.value("imageUrl").toString();
            rankedRecipes.append({r, matches});
        }
    }

    std::sort(rankedRecipes.begin(), rankedRecipes.end(), [](const RankedRecipe &a, const RankedRecipe &b) {
        return a.matchCount > b.matchCount;
    });

    beginResetModel();
    recipes_.clear();
    for (const auto &ranked : rankedRecipes) recipes_.append(ranked.item);
    endResetModel();
}

// Вспомогательный метод для получения данных по имени роли
QVariant RecipeModel::getVal(int row, const QString &roleName) {
    if (row < 0 || row >= recipes_.size()) return QVariant();
    QHash<int, QByteArray> roles = roleNames();
    int roleKey = -1;
    QHashIterator<int, QByteArray> i(roles);
    while (i.hasNext()) {
        i.next();
        if (i.value() == roleName.toUtf8()) { roleKey = i.key(); break; }
    }
    return data(index(row, 0), roleKey);
}

// Фильтр по списку тегов
void RecipeModel::filterByTags(const QStringList& tags)
{
    beginResetModel();
    recipes_ = service_->filterByTags(tags);
    endResetModel();
}

// Поиск индекса строки по названию блюда
int RecipeModel::getRowByTitle(const QString &title) {
    for (int i = 0; i < recipes_.size(); ++i) {
        if (recipes_.at(i).title == title) return i;
    }
    return -1;
}

// Живой поиск по названию
void RecipeModel::searchByTitle(const QString& text) {
    beginResetModel();
    recipes_ = service_->searchByTitle(text);
    endResetModel();
}

// ГЛАВНЫЙ ФИЛЬТР (Теги + Калории вместе)
void RecipeModel::applyFilters(const QStringList &tags, int maxCal) {
    m_activeTags = tags;
    m_maxCalories = maxCal;

    beginResetModel();
    QList<Recipe> allRecipes = service_->getAllRecipes();
    recipes_.clear();

    for (const Recipe &recipe : allRecipes) {
        bool calMatch = false;
        if (m_maxCalories == 300) calMatch = (recipe.calories <= 300);
        else if (m_maxCalories == 600) calMatch = (recipe.calories > 300 && recipe.calories <= 600);
        else calMatch = true;

        bool tagsMatch = true;
        QString recipeTags = recipe.tags.toLower();
        for (const QString &tag : m_activeTags) {
            if (!recipeTags.contains(tag.trimmed().toLower())) {
                tagsMatch = false;
                break;
            }
        }

        if (calMatch && tagsMatch) recipes_.append(recipe);
    }
    endResetModel();
}

// ТОЧЕЧНОЕ ОБНОВЛЕНИЕ ЛАЙКА (чтобы список не прыгал)
void RecipeModel::toggleFavoriteInModel(int row) {
    if (row < 0 || row >= recipes_.size()) return;

    QString title = recipes_.at(row).title;
    DatabaseManager::instance().toggleFavorite(title);

    // ГЛАВНОЕ: сообщаем QML, что данные изменились, НЕ вызывая Reset.
    // Это обновит только сердечко, фильтры останутся активными.
    emit dataChanged(index(row, 0), index(row, 0));
}

// ФИЛЬТР ДЛЯ СТРАНИЦЫ ИЗБРАННОГО
void RecipeModel::filterByFavorites(const QStringList &titles) {
    beginResetModel();
    QList<Recipe> allRecipes = service_->getAllRecipes();
    recipes_.clear();

    // Мы идем по списку заголовков, который БД вернула с ORDER BY rowid DESC
    for (const QString &title : titles) {
        for (const Recipe &recipe : allRecipes) {
            if (recipe.title == title) {
                recipes_.append(recipe);
                break;
            }
        }
    }
    endResetModel();
}