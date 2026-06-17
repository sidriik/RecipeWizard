#include "RecipeService.h"

RecipeService::RecipeService(RecipeRepository* repository, QObject* parent)
    : QObject(parent), repository_(repository)
{
}
QList<Recipe> RecipeService::getAllRecipes()
{
    return repository_->getAllRecipes();
}

bool RecipeService::addRecipe(const Recipe& recipe)
{
    return repository_->addRecipe(recipe);
}

bool RecipeService::updateRecipe(const Recipe& recipe)
{
    return repository_->updateRecipe(recipe);
}

bool RecipeService::deleteRecipe(int id)
{
    return repository_->deleteRecipe(id);
}

Recipe RecipeService::getRecipeById(int id)
{
    return repository_->getRecipeById(id);
}

bool RecipeService::recipeExists(const QString& title)
{
    return repository_->recipeExists(title);
}

QList<Recipe> RecipeService::searchRecipes(
    const QString& ingredientsText)
{
    QList<Recipe> recipes =
        repository_->getAllRecipes();

    if (ingredientsText.trimmed().isEmpty()) {
        return recipes;
    }

    QList<Recipe> result;

    QStringList ingredients =
        ingredientsText.split(",",
                              Qt::SkipEmptyParts);

    for (const Recipe& recipe : recipes) {

        bool match = false;

        for (const QString& ingredient :
             ingredients) {

            if (recipe.ingredients.contains(
                    ingredient.trimmed(),
                    Qt::CaseInsensitive))
            {
                match = true;
                break;
            }
        }

        if (match) {
            result.append(recipe);
        }
    }

    return result;
}

QList<Recipe> RecipeService::findBestRecipes(
    const QStringList& availableIngredients)
{
    QList<Recipe> recipes =
        repository_->getAllRecipes();

    QList<Recipe> result;

    int minMissing = INT_MAX;

    for (const Recipe& recipe : recipes) {

        QStringList recipeIngredients =
            recipe.ingredients.split(
                ",",
                Qt::SkipEmptyParts);

        int missingCount = 0;

        for (QString ingredient :
             recipeIngredients)
        {
            ingredient = ingredient.trimmed();

            bool found = false;

            for (QString available :
                 availableIngredients)
            {
                if (ingredient.compare(
                        available.trimmed(),
                        Qt::CaseInsensitive) == 0)
                {
                    found = true;
                    break;
                }
            }

            if (!found) {
                missingCount++;
            }
        }

        if (missingCount < minMissing) {

            minMissing = missingCount;

            result.clear();
            result.append(recipe);
        }
        else if (missingCount == minMissing)
        {
            result.append(recipe);
        }
    }

    return result;
}

QList<Recipe> RecipeService::filterByTags(
    const QStringList& tags)
{
    QList<Recipe> result;

    for (const Recipe& recipe :
         repository_->getAllRecipes())
    {
        bool match = true;

        for (const QString& tag : tags)
        {
            if (!recipe.tags.contains(
                    tag.trimmed(),
                    Qt::CaseInsensitive))
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            result.append(recipe);
        }
    }

    return result;
}

QString RecipeService::getAIAnalysis(const QString &title, const QString &tags, int calories) {
    QString t = tags.toLower();
    QString name = title.toLower();

    // 1. Логика для Салатов
    if (t.contains("салат") || name.contains("салат")) {
        return "🥗 Анализ: Блюдо богато живой клетчаткой. \nИИ-Совет: Используйте нерафинированное оливковое масло, чтобы витамины из овощей усвоились на 100%.";
    }

    // 2. Логика для Завтраков
    if (t.contains("завтрак")) {
        if (calories < 300)
            return "☀️ Анализ: Легкий заряд энергии. \nИИ-Совет: Добавьте горсть грецких орехов, чтобы чувство сытости сохранилось до самого обеда.";
        return "💪 Анализ: Плотный питательный завтрак. \nИИ-Совет: Стакан воды с лимоном за 15 минут до еды поможет запустить метаболизм.";
    }

    // 3. Логика для Супов
    if (t.contains("суп") || name.contains("суп")) {
        return "🥣 Анализ: Легкое усвоение и водно-солевой баланс. \nИИ-Совет: Добавьте немного свежей зелени (петрушка, укроп) в тарелку перед подачей для сохранения витамина С.";
    }

    // 4. Логика для десертов / сладкого
    if (t.contains("десерт") || t.contains("банан")) {
        return "🍎 Анализ: Натуральные сахара. \nИИ-Совет: Посыпьте блюдо корицей — она помогает контролировать уровень сахара в крови.";
    }

    // 5. Общая логика по калориям (если ничего не подошло)
    if (calories > 600) {
        return "🥘 Анализ: Высокая энергетическая ценность. \nИИ-Совет: Разделите порцию или сделайте упор на овощной гарнир, чтобы избежать тяжести.";
    }

    return "🌱 Анализ: Сбалансированный состав. \nИИ-Совет: Наслаждайтесь каждым кусочком, осознанное пережевывание улучшает усвоение нутриентов.";
}

double RecipeService::calculateDailyCalories(double weight, double height, int age, bool isMale, double activityFactor) {
    // Формула Миффлина-Сан Жеора
    double bmr = (10 * weight) + (6.25 * height) - (5 * age);
    bmr = isMale ? (bmr + 5) : (bmr - 161);
    return bmr * activityFactor;
}

QString RecipeService::calculateFullNutrients(double weight, double height, int age, bool isMale, double activity) {
    double bmr = (10 * weight) + (6.25 * height) - (5 * age);
    bmr = isMale ? (bmr + 5) : (bmr - 161);
    double totalCalories = bmr * activity;

    // Рассчитываем БЖУ (стандарт: 30% белки, 30% жиры, 40% углеводы)
    double p = (totalCalories * 0.30) / 4; // 1г белка = 4 ккал
    double f = (totalCalories * 0.30) / 9; // 1г жира = 9 ккал
    double c = (totalCalories * 0.40) / 4; // 1г углев = 4 ккал

    return QString::number(totalCalories,'f',0) + "|" +
           QString::number(p,'f',1) + "|" +
           QString::number(f,'f',1) + "|" +
           QString::number(c,'f',1);
}

#include <QRegularExpression>

QString RecipeService::scaleIngredients(const QString &ingredients, double multiplier) {
    if (multiplier == 1.0) return ingredients;

    // Регулярное выражение: ищет целые числа и дроби (типа 2 или 1.5 или 1,5)
    QRegularExpression re("(\\d+([\\.,]\\d+)?)");
    QString result = ingredients;
    QRegularExpressionMatchIterator i = re.globalMatch(ingredients);

    // Собираем изменения с конца строки к началу, чтобы индексы не поплыли
    struct Replacement { int pos; int len; QString val; };
    QList<Replacement> replacements;

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString numStr = match.captured(1).replace(',', '.'); // заменяем запятую на точку для расчета
        double num = numStr.toDouble();
        double scaledNum = num * multiplier;

        // Форматируем число: 1 знак после запятой, убираем лишние нули
        QString newNum = QString::number(scaledNum, 'f', 1);
        if (newNum.endsWith(".0")) newNum.chop(2);

        replacements.prepend({(int)match.capturedStart(), (int)match.capturedLength(), newNum});
    }

    for (const auto &r : replacements) {
        result.replace(r.pos, r.len, r.val);
    }

    return result;
}

QList<Recipe> RecipeService::searchByTitle(const QString& titleText) {
    QList<Recipe> all = repository_->getAllRecipes();
    if (titleText.trimmed().isEmpty()) return all;

    QList<Recipe> result;
    for (const Recipe& r : all) {
        if (r.title.contains(titleText, Qt::CaseInsensitive)) {
            result.append(r);
        }
    }
    return result;
}

QString RecipeService::getCleanName(const QString &ingredient) {
    QString name = ingredient.trimmed();

    // 1. Удаляем все цифры, дроби (1/2, 0.5) и технические символы (+, -, заменяя их на пробелы)
    name.replace(QRegularExpression("[0-9.,\\/+\\-]+"), " ");

    // 2. Расширенный список единиц измерения и мусорных слов
    QStringList units = {
        "ст\\.?\\s*л\\.?", "ч\\.?\\s*л\\.?", "ложка", "ложки", "ложек",
        "грамм", "гр", "г", "килограмм", "кг", "миллилитр", "мл", "литр", "л",
        "шт\\.?", "штука", "штуки", "штук", "банка", "банки", "банок",
        "зубчик", "зубчика", "зубчиков", "стакан", "стакана", "стаканов",
        "щепотка", "щепотки", "щепоток", "по вкусу", "упаковка", "упаковки", "упаковок"
    };

    // Пробегаемся по списку единиц измерения и безжалостно вырезаем их (с учетом пробелов вокруг)
    for (const QString &unit : units) {
        QRegularExpression re("(?:^|\\s)" + unit + "(?:\\s|$)", QRegularExpression::CaseInsensitiveOption);
        name.replace(re, " ");
    }

    // 3. Схлопываем лишние пробелы, которые могли образоваться после удаления
    name.replace(QRegularExpression("\\s+"), " ");
    name = name.trimmed();

    // 4. Делаем первую букву заглавной, чтобы в корзине всё выглядело аккуратно
    if (!name.isEmpty()) {
        name[0] = name[0].toUpper();
    }

    return name;
}

#include <QRegularExpression>

QStringList RecipeService::getStepList(const QString &instructions) {
    return instructions.split("\n", Qt::SkipEmptyParts);
}

int RecipeService::getTimerSeconds(const QString &stepText) {
    // Ищем число перед словами "мин" или "минут"
    QRegularExpression re("(\\d+)\\s*(мин|минут)");
    QRegularExpressionMatch match = re.match(stepText);
    if (match.hasMatch()) {
        return match.captured(1).toInt() * 60; // Превращаем минуты в секунды
    }
    return 0; // Если времени нет
}