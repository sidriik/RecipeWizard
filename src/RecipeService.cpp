#include "RecipeService.h"

#include <algorithm>

#include <QRegularExpression>

RecipeService::RecipeService(RecipeRepository *repository, QObject *parent)
    : QObject(parent), repository_(repository)
{
}

QList<Recipe> RecipeService::getAllRecipes()
{
    return repository_->getAllRecipes();
}

bool RecipeService::addRecipe(const Recipe &recipe)
{
    return repository_->addRecipe(recipe);
}

Recipe RecipeService::getRecipeById(int id)
{
    return repository_->getRecipeById(id);
}

bool RecipeService::recipeExists(const QString &title)
{
    return repository_->recipeExists(title);
}

QList<Recipe> RecipeService::searchRecipes(const QString &ingredientsText)
{
    QList<Recipe> recipes = repository_->getAllRecipes();
    if (ingredientsText.trimmed().isEmpty())
        return recipes;

    QList<Recipe> result;
    const QStringList ingredients = ingredientsText.split(QLatin1Char(','), Qt::SkipEmptyParts);
    for (const Recipe &recipe : recipes) {
        for (const QString &ingredient : ingredients) {
            if (recipe.ingredients.contains(ingredient.trimmed(), Qt::CaseInsensitive)) {
                result.append(recipe);
                break;
            }
        }
    }
    return result;
}

QList<Recipe> RecipeService::findBestRecipes(const QStringList &availableIngredients)
{
    struct Ranked
    {
        Recipe recipe;
        int matches = 0;
    };

    QList<Ranked> ranked;
    for (const Recipe &recipe : repository_->getAllRecipes()) {
        const QString composition = recipe.ingredients.toLower();
        int matches = 0;
        for (const QString &available : availableIngredients) {
            const QString clean = available.trimmed().toLower();
            if (!clean.isEmpty() && composition.contains(clean))
                ++matches;
        }
        if (matches > 0)
            ranked.append({recipe, matches});
    }

    std::sort(ranked.begin(), ranked.end(),
              [](const Ranked &a, const Ranked &b) { return a.matches > b.matches; });

    QList<Recipe> result;
    result.reserve(ranked.size());
    for (const Ranked &item : ranked)
        result.append(item.recipe);
    return result;
}

QList<Recipe> RecipeService::filterByTags(const QStringList &tags)
{
    QList<Recipe> result;
    for (const Recipe &recipe : repository_->getAllRecipes()) {
        bool match = true;
        for (const QString &tag : tags) {
            if (!recipe.tags.contains(tag.trimmed(), Qt::CaseInsensitive)) {
                match = false;
                break;
            }
        }
        if (match)
            result.append(recipe);
    }
    return result;
}

QList<Recipe> RecipeService::searchByTitle(const QString &titleText)
{
    QList<Recipe> all = repository_->getAllRecipes();
    if (titleText.trimmed().isEmpty())
        return all;

    QList<Recipe> result;
    for (const Recipe &recipe : all) {
        if (recipe.title.contains(titleText, Qt::CaseInsensitive))
            result.append(recipe);
    }
    return result;
}

QString RecipeService::getRecipeAdvice(const QString &title, const QString &tags, int calories)
{
    const QString t = tags.toLower();
    const QString name = title.toLower();

    if (t.contains(QStringLiteral("салат")) || name.contains(QStringLiteral("салат")))
        return QStringLiteral(
            "🥗 Анализ: блюдо богато живой клетчаткой.\nСовет: используйте "
            "нерафинированное оливковое масло, чтобы витамины усвоились лучше.");

    if (t.contains(QStringLiteral("завтрак"))) {
        if (calories < 300)
            return QStringLiteral(
                "☀️ Анализ: лёгкий заряд энергии.\nСовет: добавьте горсть "
                "грецких орехов, чтобы сытость сохранилась до обеда.");
        return QStringLiteral(
            "💪 Анализ: плотный питательный завтрак.\nСовет: стакан воды с "
            "лимоном за 15 минут до еды поможет запустить метаболизм.");
    }

    if (t.contains(QStringLiteral("суп")) || name.contains(QStringLiteral("суп")))
        return QStringLiteral(
            "🥣 Анализ: лёгкое усвоение и водно-солевой баланс.\nСовет: добавьте "
            "свежую зелень перед подачей, чтобы сохранить витамин C.");

    if (t.contains(QStringLiteral("десерт")) || t.contains(QStringLiteral("банан")))
        return QStringLiteral(
            "🍎 Анализ: натуральные сахара.\nСовет: посыпьте блюдо корицей — "
            "она помогает контролировать уровень сахара в крови.");

    if (calories > 600)
        return QStringLiteral(
            "🥘 Анализ: высокая энергетическая ценность.\nСовет: разделите "
            "порцию или добавьте овощной гарнир, чтобы избежать тяжести.");

    return QStringLiteral(
        "🌱 Анализ: сбалансированный состав.\nСовет: осознанное пережёвывание "
        "улучшает усвоение нутриентов.");
}

double RecipeService::calculateDailyCalories(double weight, double height, int age,
                                             bool isMale, double activityFactor)
{
    double bmr = (10.0 * weight) + (6.25 * height) - (5.0 * age);
    bmr += isMale ? 5.0 : -161.0;
    return bmr * activityFactor;
}

QString RecipeService::calculateFullNutrients(double weight, double height, int age,
                                              bool isMale, double activity)
{
    const double totalCalories = calculateDailyCalories(weight, height, age, isMale, activity);
    const double proteins = (totalCalories * 0.30) / 4.0;
    const double fats = (totalCalories * 0.30) / 9.0;
    const double carbs = (totalCalories * 0.40) / 4.0;
    return QStringLiteral("%1|%2|%3|%4")
        .arg(QString::number(totalCalories, 'f', 0),
             QString::number(proteins, 'f', 1),
             QString::number(fats, 'f', 1),
             QString::number(carbs, 'f', 1));
}

QString RecipeService::scaleIngredients(const QString &ingredients, double multiplier)
{
    if (qFuzzyCompare(multiplier, 1.0))
        return ingredients;

    static const QRegularExpression number(QStringLiteral("(\\d+([\\.,]\\d+)?)"));
    QString result = ingredients;
    QList<QRegularExpressionMatch> matches;
    auto it = number.globalMatch(ingredients);
    while (it.hasNext())
        matches.prepend(it.next());

    for (const QRegularExpressionMatch &match : matches) {
        QString numStr = match.captured(1);
        numStr.replace(QLatin1Char(','), QLatin1Char('.'));
        QString scaled = QString::number(numStr.toDouble() * multiplier, 'f', 1);
        if (scaled.endsWith(QStringLiteral(".0")))
            scaled.chop(2);
        result.replace(match.capturedStart(), match.capturedLength(), scaled);
    }
    return result;
}

QString RecipeService::getCleanName(const QString &ingredient)
{
    QString name = ingredient.trimmed();
    name.replace(QRegularExpression(QStringLiteral("[0-9.,/+\\-]+")), QStringLiteral(" "));

    static const QStringList units = {
                                      QStringLiteral("ст\\.?\\s*л\\.?"), QStringLiteral("ч\\.?\\s*л\\.?"),
                                      QStringLiteral("ложка"), QStringLiteral("ложки"), QStringLiteral("ложек"),
                                      QStringLiteral("грамм"), QStringLiteral("гр"), QStringLiteral("г"),
                                      QStringLiteral("килограмм"), QStringLiteral("кг"), QStringLiteral("миллилитр"),
                                      QStringLiteral("мл"), QStringLiteral("литр"), QStringLiteral("л"),
                                      QStringLiteral("шт\\.?"), QStringLiteral("штука"), QStringLiteral("штуки"),
                                      QStringLiteral("штук"), QStringLiteral("банка"), QStringLiteral("банки"),
                                      QStringLiteral("банок"), QStringLiteral("зубчик"), QStringLiteral("зубчика"),
                                      QStringLiteral("зубчиков"), QStringLiteral("стакан"), QStringLiteral("стакана"),
                                      QStringLiteral("стаканов"), QStringLiteral("щепотка"), QStringLiteral("щепотки"),
                                      QStringLiteral("щепоток"), QStringLiteral("по вкусу"), QStringLiteral("упаковка"),
                                      QStringLiteral("упаковки"), QStringLiteral("упаковок")};

    for (const QString &unit : units) {
        QRegularExpression re(QStringLiteral("(?:^|\\s)") + unit + QStringLiteral("(?:\\s|$)"),
                              QRegularExpression::CaseInsensitiveOption);
        name.replace(re, QStringLiteral(" "));
    }

    name.replace(QRegularExpression(QStringLiteral("\\s+")), QStringLiteral(" "));
    name = name.trimmed();
    if (!name.isEmpty())
        name[0] = name[0].toUpper();
    return name;
}

QStringList RecipeService::getStepList(const QString &instructions)
{
    return instructions.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
}

int RecipeService::getTimerSeconds(const QString &stepText)
{
    static const QRegularExpression re(QStringLiteral("(\\d+)\\s*(мин|минут)"));
    const QRegularExpressionMatch match = re.match(stepText);
    if (match.hasMatch())
        return match.captured(1).toInt() * 60;
    return 0;
}
