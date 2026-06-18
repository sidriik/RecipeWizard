#include "DatabaseManager.h"

#include <QSqlError>
#include <QSqlQuery>

#include "RecipeException.h"

namespace {
void execOrThrow(QSqlQuery &query)
{
    if (!query.exec())
        throw RecipeException(query.lastError().text());
}

}

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager manager;
    return manager;
}

void DatabaseManager::init(const QString &databasePath, const QString &imageBase)
{
    m_imageBase = imageBase;
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    m_db.setDatabaseName(databasePath);
    if (!m_db.open())
        throw RecipeException(m_db.lastError().text());

    createTables();
    seedData();
}

void DatabaseManager::createTables()
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS recipes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "ingredients TEXT NOT NULL,"
        "tags TEXT,"
        "calories INTEGER,"
        "proteins REAL,"
        "fats REAL,"
        "carbs REAL,"
        "servings INTEGER DEFAULT 4,"
        "instructions TEXT,"
        "imageUrl TEXT)"));
    execOrThrow(query);

    query.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS favorites (recipe_title TEXT UNIQUE)"));
    execOrThrow(query);

    query.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS history ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "recipe_title TEXT,"
        "viewed_at DATETIME DEFAULT CURRENT_TIMESTAMP)"));
    execOrThrow(query);

    query.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS shopping_list ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE NOT NULL)"));
    execOrThrow(query);

    query.prepare(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS fridge_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE)"));
    execOrThrow(query);
}

void DatabaseManager::seedData()
{
    QSqlQuery check(QStringLiteral("SELECT COUNT(*) FROM recipes"), m_db);
    execOrThrow(check);
    if (!check.next() || check.value(0).toInt() != 0)
        return;

    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO recipes "
        "(title, ingredients, tags, calories, proteins, fats, carbs, servings, "
        "instructions, imageUrl) VALUES "
        "(:title, :ing, :tags, :cal, :prot, :fat, :carb, :serv, :inst, :img)"));

    auto add = [&](const QString &title, const QString &ing, const QString &tags,
                   int cal, double p, double f, double c, int s,
                   const QString &inst, const QString &img) {
        query.bindValue(QStringLiteral(":title"), title);
        query.bindValue(QStringLiteral(":ing"), ing);
        query.bindValue(QStringLiteral(":tags"), tags);
        query.bindValue(QStringLiteral(":cal"), cal);
        query.bindValue(QStringLiteral(":prot"), p);
        query.bindValue(QStringLiteral(":fat"), f);
        query.bindValue(QStringLiteral(":carb"), c);
        query.bindValue(QStringLiteral(":serv"), s);
        query.bindValue(QStringLiteral(":inst"), inst);
        if (img.startsWith(QStringLiteral("http")))
            query.bindValue(QStringLiteral(":img"), img);
        else
            query.bindValue(QStringLiteral(":img"), m_imageBase + img.trimmed());
        execOrThrow(query);
    };

    add("Омлет с сыром и шпинатом", "2 шт. Яйца куриные, 50г Сыр Чеддер, 30г Шпинат свежий, 40мл Молоко 3.2%, 10г Сливочное масло", "завтрак, весна, без сахара, быстро", 320, 20.0, 24.0, 4.5, 1,
        "1. Взбейте 2 яйца с молоком и щепоткой соли.\n2. Нарежьте шпинат и добавьте в смесь.\n3. Разогрейте сковороду с маслом.\n4. Вылейте смесь и жарьте 5 минут на медленном огне.\n5. Посыпьте тертым сыром и сложите пополам.", "1.jpg");
    add("Сырники с ванилью", "300г Творог 5%, 1 шт. Яйцо, 40г Мука рисовая, 10г Сахар тростниковый, 1г Ванилин", "завтрак, зима, высокобелковое, без глютена", 385, 34.0, 12.0, 22.0, 2,
        "1. Разомните творог вилкой, смешайте с яйцом и сахаром.\n2. Постепенно всыпьте рисовую муку и ванилин.\n3. Сформируйте небольшие шайбочки.\n4. Обжаривайте на среднем огне по 4 минуты с каждой стороны.", "2.jpg");
    add("Овсянка с карамелью", "60г Хлопья овсяные, 200мл Фундучное молоко, 1 шт. Банан, 15г Мед, 10г Грецкий орех", "завтрак, осень, вегетарианское", 315, 7.0, 9.0, 48.0, 1,
        "1. Залейте хлопья молоком и варите 8 минут до густоты.\n2. Нарежьте банан кружочками.\n3. Прогрейте банан на сковороде с медом 3 минуты.\n4. Выложите кашу в миску, сверху — бананы и орехи.", "3.jpg");
    add("Шакшука по-домашнему", "3 шт. Яйца, 200г Томаты в соку, 100г Болгарский перец, 1 зубчик Чеснок, 10мл Масло оливковое", "завтрак, лето, кето, высокобелковое", 290, 19.0, 15.0, 12.0, 1,
        "1. Перец нарежьте кубиками и жарьте 6 минут.\n2. Добавьте томаты и чеснок, тушите еще 5 минут.\n3. Сделайте лунки и вбейте яйца.\n4. Готовьте под крышкой 8 минут до застывания белка.", "4.jpg");
    add("Блинчики на кефире", "250мл Кефир, 150г Мука, 1 шт. Яйцо, 20г Сахар, 3г Сода", "завтрак, весна, вегетарианское", 410, 10.0, 8.0, 60.0, 3,
        "1. Смешайте кефир, яйцо и сахар.\n2. Всыпьте муку с содой и перемешайте венчиком.\n3. Дайте тесту постоять 10 минут.\n4. Выпекайте на раскаленной сковороде до золотистого цвета.", "5.jpg");
    add("Тост с лососем", "1 шт. Хлеб злаковый, 50г Лосось слабосоленый, 30г Творожный сыр, 0.5 шт. Огурец", "завтрак, лето, пп, без сахара", 240, 14.0, 12.0, 18.0, 1,
        "1. Подсушите хлеб в тостере.\n2. Намажьте творожным сыром.\n3. Сверху выложите слайсы огурца и ломтики лосося.\n4. Посыпьте черным перцем.", "6.jpg");
    add("Смузи-боул с манго", "1 шт. Банан, 100г Замороженное манго, 100мл Кокосовое молоко, 10г Гранола", "завтрак, лето, веган, без глютена", 270, 4.0, 10.0, 42.0, 1,
        "1. Поместите банан, манго и молоко в блендер.\n2. Взбивайте на высокой скорости 2 минуты.\n3. Перелейте в глубокую миску.\n4. Украсьте гранолой и свежими ягодами.", "7.jpg");
    add("Рисовая каша с кокосом", "50г Рис, 200мл Кокосовое молоко, 10г Кокосовая стружка, 5г Сахар", "завтрак, зима, вегетарианское, без глютена", 295, 5.0, 12.0, 44.0, 1,
        "1. Промойте рис и залейте молоком.\n2. Варите на медленном огне 15 минут, помешивая.\n3. Добавьте сахар и щепотку соли.\n4. При подаче посыпьте кокосовой стружкой.", "8.jpg");
    add("Яичница в облаке", "2 шт. Яйца, 1 ломтик Хлеб, 10г Пармезан, 5г Зелень", "завтрак, лето, быстро, пп", 210, 14.0, 12.0, 10.0, 1,
        "1. Отделите белки. Взбейте их в пену.\n2. Выложите белки на противень, в центр — желток.\n3. Запекайте в духовке 5 минут при 180 градусах.\n4. Положите на тост и посыпьте пармезаном.", "9.jpg");
    add("Творог с орехами", "200г Творог 5%, 20г Миндаль, 10г Изюм, 5мл Сироп агавы", "завтрак, осень, высокобелковое, без сахара", 340, 30.0, 14.0, 18.0, 1,
        "1. Выложите творог в пиалу.\n2. Добавьте изюм и рубленый миндаль.\n3. Полейте сиропом.\n4. Тщательно перемешайте.", "10.jpg");
    add("Борщ «Осознанный»", "400г Говядина, 300г Свекла, 200г Капуста, 3 шт. Картофель, 1 шт. Морковь, 1 шт. Лук, 2 ст.л. Томатная паста", "обед, зима, сытное", 245, 26.0, 10.0, 25.0, 4,
        "1. Мясо варить 90 минут до мягкости.\n2. Натрите свеклу и морковь, обжарьте с пастой 15 минут.\n3. Добавьте овощи в бульон и варите 20 минут.\n4. Дайте настояться 15 минут перед подачей.", "11.jpg");
    add("Тыквенный суп-пюре", "600г Тыква, 150мл Сливки 10%, 1 шт. Лук, 30г Семечки, 1 зубчик Чеснок", "обед, осень, вегетарианское, без глютена", 210, 5.0, 14.0, 22.0, 3,
        "1. Нарежьте тыкву и лук, варите 20 минут.\n2. Слейте воду, добавьте сливки и чеснок.\n3. Взбейте блендером до гладкости.\n4. Посыпьте семечками.", "12.jpg");
    add("Куриное филе на гриле", "250г Куриная грудка, 100г Брокколи, 10мл Оливковое масло, 5г Прованские травы", "обед, лето, высокобелковое, пп", 320, 45.0, 8.0, 4.0, 1,
        "1. Курицу натрите специями и маслом.\n2. Жарьте на гриле по 6 минут с каждой стороны.\n3. Брокколи отварите в кипятке 4 минуты.\n4. Подавайте с долькой лимона.", "13.jpg");
    add("Паста Карбонара", "250г Спагетти, 100г Бекон, 2 шт. Желтки, 40г Пармезан, 5г Черный перец", "обед, ужин, зима, итальянское", 610, 24.0, 32.0, 58.0, 2,
        "1. Варите спагетти 8 минут до состояния аль-денте.\n2. Бекон обжарьте 7 минут до хруста.\n3. Смешайте желтки с пармезаном.\n4. Смешайте пасту с беконом и влейте соус, быстро перемешивая.", "14.jpg");
    add("Салат Цезарь", "150г Курица, 100г Салат Айсберг, 20г Пармезан, 15г Сухарики, 30мл Соус Цезарь", "обед, лето, быстро, высокобелковое", 430, 30.0, 25.0, 12.0, 1,
        "1. Обжарьте курицу 10 минут.\n2. Нарвите салат в миску.\n3. Смешайте с соусом, добавьте курицу и сыр.\n4. Посыпьте сухариками.", "15.jpg");
    add("Рыбная запеканка", "300г Филе трески, 2 шт. Картофель, 50г Сыр, 50мл Сливки", "обед, ужин, весна, без глютена", 340, 28.0, 12.0, 26.0, 2,
        "1. Нарежьте картофель тонкими слайсами.\n2. Выложите в форму слоями: картофель, рыбу, картофель.\n3. Залейте сливками и посыпьте сыром.\n4. Запекайте 35 минут при 180 градусах.", "16.jpg");
    add("Индейка с гречкой", "200г Филе индейки, 100г Гречка, 1 шт. Морковь, 5г Масло", "обед, перекус, осень, пп", 380, 40.0, 6.0, 42.0, 1,
        "1. Отварите гречку (варить 15 минут).\n2. Индейку нарежьте кусочками и тушите с морковью 20 минут.\n3. Соедините гречку с мясом и добавьте масло.", "17.jpg");
    add("Суп Минестроне", "1 шт. Кабачок, 1 шт. Морковь, 100г Фасоль консервир., 2 шт. Томаты, 10г Базилик", "обед, лето, веган, без глютена", 180, 8.0, 3.0, 32.0, 3,
        "1. Нарежьте овощи кубиками.\n2. Жарьте лук и морковь 5 минут.\n3. Добавьте остальные овощи и воду, варите 20 минут.\n4. В конце добавьте фасоль и базилик.", "18.jpg");
    add("Стейк из тунца", "200г Тунец стейк, 150г Микс салата, 10мл Соевый соус, 5г Кунжут", "обед, ужин, весна, кето", 280, 46.0, 8.0, 1.0, 1,
        "1. Сбрызните тунец соусом.\n2. Жарьте на сильном огне ровно по 2 минуты с каждой стороны.\n3. Подавайте на подушке из салата с кунжутом.", "19.jpg");
    add("Рис с овощами по-азиатски", "150г Рис, 100г Замороженные овощи, 20мл Соевый соус, 1 шт. Яйцо", "обед, ужин, лето, вегетарианское, быстро", 350, 10.0, 6.0, 62.0, 2,
        "1. Отварите рис до готовности.\n2. Обжарьте овощи на сковороде 5 минут.\n3. Вбейте в овощи яйцо и перемешайте.\n4. Добавьте рис и соус, жарьте еще 3 минуты.", "20.jpg");
    add("Грибное ризотто", "150г Рис Арборио, 200г Шампиньоны, 50мл Белое вино, 30г Пармезан, 500мл Бульон", "обед, ужин, осень, итальянское", 480, 12.0, 18.0, 65.0, 2,
        "1. Обжарьте грибы 10 минут.\n2. Добавьте рис и жарьте 2 минуты до прозрачности.\n3. Влейте вино и дайте выпариться.\n4. Постепенно вливайте бульон, помешивая (варить 18 минут).", "21.jpg");
    add("Суп из чечевицы", "150г Чечевица красная, 1 шт. Лук, 1 шт. Морковь, 10г Куркума, 1 шт. Лимон", "обед, осень, веган, без глютена", 260, 18.0, 2.0, 45.0, 3,
        "1. Чечевицу промойте и залейте водой.\n2. Варите 15 минут на среднем огне.\n3. Добавьте обжаренный лук и морковь со специями.\n4. Варите еще 10 минут, при подаче выжмите лимон.", "22.jpg");
    add("Котлеты из нута", "200г Нут вареный, 1 шт. Лук, 30г Кинза, 20г Мука нутовая", "обед, весна, веган, высокобелковое", 310, 16.0, 7.0, 48.0, 2,
        "1. Нут и лук измельчите в блендере.\n2. Добавьте муку и рубленую кинзу.\n3. Сформируйте котлеты.\n4. Запекайте в духовке 25 минут при 190 градусах.", "23.jpg");
    add("Пельмени домашние", "300г Фарш говяжий, 200г Тесто, 1 шт. Лук, 5г Перец", "обед, зима, сытное", 620, 22.0, 30.0, 55.0, 2,
        "1. Смешайте фарш с мелко нарезанным луком.\n2. Сформируйте пельмени из теста и начинки.\n3. Бросьте в кипящую воду.\n4. Варите 7 минут после закипания.", "24.jpg");
    add("Греческий салат с фетой", "2 шт. Томаты, 1 шт. Огурец, 50г Сыр Фета, 10 шт. Маслины, 15мл Масло", "обед, ужин, лето, вегетарианское, быстро", 210, 7.0, 18.0, 10.0, 2,
        "1. Нарежьте овощи крупными кубиками.\n2. Добавьте маслины и кубики феты.\n3. Заправьте оливковым маслом.\n4. Посыпьте сушеным орегано.", "25.jpg");
    add("Лосось с аспарагусом", "200г Филе лосося, 100г Спаржа, 10г Масло, 0.5 шт. Лимон", "ужин, весна, кето, без сахара", 420, 32.0, 34.0, 1.5, 1,
        "1. Посолите рыбу и жарьте на масле по 4 минуты с каждой стороны.\n2. Спаржу обжарьте на той же сковороде 5 минут.\n3. Сбрызните лимонным соком перед подачей.", "26.jpg");
    add("Говядина с брокколи", "250г Говядина, 150г Брокколи, 30мл Соевый соус, 10г Имбирь", "ужин, осень, без сахара, высокобелковое", 390, 42.0, 16.0, 14.0, 1,
        "1. Мясо нарежьте соломкой, жарьте на сильном огне 5 минут.\n2. Добавьте брокколи и тертый имбирь.\n3. Залейте соусом и тушите под крышкой 10 минут.", "27.jpg");
    add("Лазанья классическая", "400г Фарш, 6 шт. Листы лазаньи, 150г Сыр Моцарелла, 200мл Соус томатный", "ужин, зима, итальянское, сытное", 680, 32.0, 38.0, 52.0, 3,
        "1. Обжарьте фарш с соусом 10 минут.\n2. Выложите в форму слоями листы и мясо.\n3. Посыпьте сыром.\n4. Запекайте в духовке 30 минут при 190 градусах.", "28.jpg");
    add("Запеченный кабачок", "2 шт. Кабачки, 50г Сыр, 1 зубчик Чеснок, 10г Сметана", "ужин, лето, вегетарианское, быстро", 190, 6.0, 12.0, 15.0, 2,
        "1. Нарежьте кабачки кружочками.\n2. Смажьте смесью сметаны и чеснока.\n3. Посыпьте сыром.\n4. Запекайте 20 минут при 200 градусах.", "29.jpg");
    add("Стейк из говядины", "300г Стейк Рибай, 10г Масло сливочное, 5г Розмарин, 1 зубчик Чеснок", "ужин, зима, кето, высокобелковое", 750, 55.0, 60.0, 0.0, 1,
        "1. Стейк обсушите салфеткой.\n2. Жарьте на раскаленной сковороде по 3 минуты с каждой стороны.\n3. Добавьте масло, розмарин и чеснок в конце.\n4. Дайте отдохнуть мясу 5 минут под фольгой.", "30.jpg");
    add("Фалафель в лаваше", "150г Фалафель, 1 шт. Лаваш, 30г Огурцы, 20г Соус тахини", "ужин, перекус, весна, веган", 420, 12.0, 18.0, 55.0, 1,
        "1. Фалафель разогрейте или обжарьте.\n2. Нарежьте огурцы соломкой.\n3. Заверните всё в лаваш и полейте соусом.\n4. Поджарьте на сухой сковороде 2 минуты.", "31.jpg");
    add("Курица в соусе Терияки", "250г Курица, 50мл Соус терияки, 100г Рис, 5г Кунжут", "ужин, осень, азиатское, быстро", 490, 38.0, 10.0, 65.0, 1,
        "1. Курицу обжарьте 7 минут.\n2. Залейте соусом и томите 3 минуты.\n3. Отварите рис (варить 12 минут).\n4. Выложите курицу на рис и посыпьте кунжутом.", "32.jpg");
    add("Паста с креветками", "200г Паста, 100г Креветки, 50мл Сливки, 1 зубчик Чеснок, 10г Сыр", "ужин, лето, итальянское, быстро", 460, 22.0, 18.0, 52.0, 1,
        "1. Варите пасту 9 минут.\n2. Креветки жарьте с чесноком 3 минуты.\n3. Влейте сливки и всыпьте сыр, прогрейте минуту.\n4. Смешайте пасту с соусом.", "33.jpg");
    add("Котлеты из индейки", "300г Фарш индейки, 1 шт. Кабачок, 1 шт. Яйцо, 10г Отруби", "ужин, зима, без сахара, пп", 310, 35.0, 12.0, 8.0, 2,
        "1. Кабачок натрите и отожмите.\n2. Смешайте с фаршем, яйцом и отрубями.\n3. Сформируйте котлеты.\n4. Запекайте в духовке 25 минут.", "34.jpg");
    add("Салат с тунцом", "1 баночка Тунец, 1 шт. Яйцо вареное, 50г Стручковая фасоль, 2 шт. Томаты", "ужин, весна, без сахара, высокобелковое", 290, 32.0, 14.0, 10.0, 1,
        "1. Смешайте кусочки тунца с нарезанным яйцом.\n2. Добавьте отваренную фасоль и томаты.\n3. Заправьте лимонным соком.", "35.jpg");
    add("Плов с сухофруктами", "150г Рис, 30г Курага, 30г Изюм, 10г Масло, 5г Корица", "ужин, осень, вегетарианское", 380, 6.0, 8.0, 72.0, 2,
        "1. Промойте рис, сухофрукты нарежьте.\n2. Обжарьте рис с сухофруктами в масле 3 минуты.\n3. Залейте водой и варите 20 минут до готовности.", "36.jpg");
    add("Картофель запеченный", "3 шт. Картофель, 10г Масло, 5г Чеснок сушеный, 5г Паприка", "ужин, зима, веган, быстро", 220, 4.0, 6.0, 38.0, 1,
        "1. Нарежьте картофель дольками.\n2. Смешайте с маслом и специями.\n3. Запекайте в духовке 30 минут при 210 градусах.", "37.jpg");
    add("Рагу из морепродуктов", "200г Мидии и Кальмары, 1 шт. Перец, 2 шт. Томаты, 10мл Вино", "ужин, лето, без сахара, пп", 240, 30.0, 8.0, 12.0, 1,
        "1. Нарежьте овощи соломкой, тушите 7 минут.\n2. Добавьте морепродукты и вино.\n3. Томите еще 5 минут под крышкой.", "38.jpg");
    add("Куриные крылышки", "400г Крылышки, 30мл Соус Барбекю, 1 зубчик Чеснок", "ужин, зима, быстро", 540, 32.0, 40.0, 15.0, 1,
        "1. Замаринуйте крылья в соусе и чесноке на 20 минут.\n2. Выложите на противень.\n3. Запекайте 35 минут при 200 градусах.", "39.jpg");
    add("Веганский бургер", "1 шт. Булочка, 100г Котлета из нута, 1 шт. Томат, 10г Горчица", "ужин, перекус, лето, веган", 410, 12.0, 15.0, 52.0, 1,
        "1. Поджарьте булочку.\n2. Котлету обжарьте по 4 минуты с каждой стороны.\n3. Соберите бургер с томатом и горчицей.", "40.jpg");
    add("Брауни на шоколаде", "100г Шоколад 70%, 2 шт. Яйца, 40г Сливочное масло, 30г Мука миндальная", "десерт, зима, без глютена", 480, 8.0, 35.0, 30.0, 4,
        "1. Растопите шоколад с маслом на водяной бане.\n2. Смешайте с яйцами и мукой.\n3. Запекайте 20 минут при 170 градусах.", "41.jpg");
    add("Яблочный пирог", "3 шт. Яблоки, 2 шт. Яйца, 100г Мука, 50г Сахар", "десерт, осень, вегетарианское", 320, 5.0, 4.0, 65.0, 6,
        "1. Нарежьте яблоки.\n2. Смешайте яйца, муку и сахар.\n3. Залейте яблоки тестом.\n4. Запекайте 40 минут.", "42.jpg");
    add("Творожный мусс", "150г Творог мягкий, 50г Клубника, 10г Мед", "перекус, весна, высокобелковое, без сахара", 190, 18.0, 4.0, 12.0, 1,
        "1. Взбейте творог с медом блендером 1 минуту.\n2. Добавьте нарезанную клубнику.\n3. Охладите 10 минут перед употреблением.", "43.jpg");
    add("Ягодный смузи", "150г Клубника заморож., 150мл Йогурт, 1 шт. Банан", "напитки, лето, быстро, без сахара", 180, 5.0, 2.0, 35.0, 1,
        "1. Поместите всё в блендер.\n2. Взбивайте на максимальной скорости 2 минуты.", "44.jpg");
    add("Имбирный чай", "500мл Вода, 20г Корень имбиря, 1 шт. Лимон, 15г Мед", "напитки, зима, быстро, без глютена", 45, 1.0, 0.1, 10.0, 2,
        "1. Натрите имбирь.\n2. Залейте кипятком и дайте настояться 10 минут.\n3. Добавьте сок лимона и мед.", "45.jpg");
    add("Банановое мороженое", "2 шт. Бананы замороженные", "десерт, лето, веган, без сахара", 190, 3.0, 1.0, 45.0, 1,
        "1. Нарежьте бананы и заморозьте.\n2. Достаньте и сразу пробейте блендером 3 минуты до состояния крема.", "46.jpg");
    add("Ореховые батончики", "100г Микс орехов, 50г Финики, 10г Какао", "перекус, десерт, осень, веган", 350, 9.0, 22.0, 32.0, 5,
        "1. Измельчите финики и орехи в блендере.\n2. Сформируйте батончики.\n3. Уберите в холод на 30 минут.", "47.jpg");
    add("Домашний лимонад", "1л Вода, 2 шт. Лимон, 30г Сахар, 10г Мята", "напитки, лето, быстро, вегетарианское", 120, 0.5, 0.1, 30.0, 4,
        "1. Выжмите сок лимонов.\n2. Смешайте с водой, сахаром и мятой.\n3. Охладите со льдом 10 минут.", "48.jpg");
    add("Морковный кекс", "2 шт. Морковь, 100г Мука, 2 шт. Яйца, 5г Разрыхлитель", "десерт, осень, вегетарианское", 280, 8.0, 10.0, 45.0, 6,
        "1. Натрите морковь на мелкой терке.\n2. Смешайте с яйцами и мукой.\n3. Выпекайте 35 минут при 180 градусах.", "49.jpg");
    add("Гуакамоле с чипсами", "1 шт. Авокадо, 1 зубчик Чеснок, 50г Чипсы кукурузные, 5мл Лимон", "перекус, весна, веган, без сахара", 450, 5.0, 32.0, 40.0, 2,
        "1. Разомните авокадо с чесноком и лимоном.\n2. Подавайте с чипсами.", "50.jpg");
    add("Запеченная груша", "2 шт. Груши, 20г Орехи, 10г Мед, 5г Корица", "десерт, осень, вегетарианское, быстро", 180, 3.0, 6.0, 35.0, 2,
        "1. Разрежьте груши пополам, удалите сердцевину.\n2. Всыпьте орехи и полейте медом.\n3. Запекайте 15 минут.", "51.jpg");
    add("Йогурт с семенами чиа", "150г Йогурт, 10г Семена чиа, 10г Орехи", "перекус, завтрак, весна, без сахара", 210, 12.0, 8.0, 24.0, 1,
        "1. Смешайте йогурт с чиа.\n2. Дайте постоять 15 минут.\n3. Посыпьте орехами.", "52.jpg");
    add("Печенье овсяное", "100г Овсяные хлопья, 1 шт. Банан, 20г Шоколад темный", "десерт, зима, веган, без сахара", 290, 6.0, 8.0, 52.0, 4,
        "1. Банан разомните в пюре, смешайте с хлопьями.\n2. Добавьте рубленый шоколад.\n3. Выложите ложкой на противень.\n4. Запекайте 12 минут.", "53.jpg");
    add("Клубничный мохито", "150мл Вода газированная, 50г Клубника, 10г Мята, 1 шт. Лайм", "напитки, лето, быстро, без сахара", 35, 0.5, 0.1, 8.0, 1,
        "1. Разомните клубнику и мяту с долькой лайма.\n2. Залейте холодной водой со льдом.", "54.jpg");
    add("Бутерброд с арахисовой пастой", "1 шт. Хлеб, 20г Арахисовая паста, 0.5 шт. Банан", "перекус, весна, быстро, веган", 280, 8.0, 14.0, 32.0, 1,
        "1. Поджарьте хлеб.\n2. Намажьте пастой.\n3. Сверху выложите кружочки банана.", "55.jpg");
}

bool DatabaseManager::addFavorite(const QString &title)
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral(
            "INSERT OR IGNORE INTO favorites(recipe_title) VALUES(:title)"));
        query.bindValue(QStringLiteral(":title"), title);
        execOrThrow(query);
        return true;
    } catch (const RecipeException &error) {
        qWarning("Не удалось добавить в избранное: %s", error.what());
        return false;
    }
}

bool DatabaseManager::removeFavorite(const QString &title)
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral(
            "DELETE FROM favorites WHERE recipe_title = :title"));
        query.bindValue(QStringLiteral(":title"), title);
        execOrThrow(query);
        return true;
    } catch (const RecipeException &error) {
        qWarning("Не удалось убрать из избранного: %s", error.what());
        return false;
    }
}

bool DatabaseManager::isFavorite(const QString &title)
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral(
            "SELECT COUNT(*) FROM favorites WHERE recipe_title = :title"));
        query.bindValue(QStringLiteral(":title"), title);
        execOrThrow(query);
        return query.next() && query.value(0).toInt() > 0;
    } catch (const RecipeException &error) {
        qWarning("Ошибка проверки избранного: %s", error.what());
        return false;
    }
}

QStringList DatabaseManager::getFavorites()
{
    QStringList result;
    try {
        QSqlQuery query(QStringLiteral(
                            "SELECT recipe_title FROM favorites ORDER BY recipe_title"), m_db);
        execOrThrow(query);
        while (query.next())
            result << query.value(0).toString();
    } catch (const RecipeException &error) {
        qWarning("Ошибка чтения избранного: %s", error.what());
    }
    return result;
}

bool DatabaseManager::toggleFavorite(const QString &title)
{
    if (isFavorite(title)) {
        removeFavorite(title);
        return false;
    }
    addFavorite(title);
    return true;
}

void DatabaseManager::addToHistory(const QString &title)
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral(
            "INSERT INTO history(recipe_title) VALUES(:title)"));
        query.bindValue(QStringLiteral(":title"), title);
        execOrThrow(query);
    } catch (const RecipeException &error) {
        qWarning("Ошибка записи истории: %s", error.what());
    }
}

QStringList DatabaseManager::getHistory()
{
    QStringList result;
    try {
        QSqlQuery query(QStringLiteral(
                            "SELECT recipe_title FROM history GROUP BY recipe_title "
                            "ORDER BY MAX(viewed_at) DESC"), m_db);
        execOrThrow(query);
        while (query.next())
            result << query.value(0).toString();
    } catch (const RecipeException &error) {
        qWarning("Ошибка чтения истории: %s", error.what());
    }
    return result;
}

bool DatabaseManager::addRecipe(const QString &title, const QString &ingredients,
                                const QString &tags, int calories, double proteins,
                                double fats, double carbs, int servings,
                                const QString &instructions)
{
    try {
        if (recipeExists(title)) {
            qWarning("Рецепт с таким названием уже существует");
            return false;
        }
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral(
            "INSERT INTO recipes "
            "(title, ingredients, tags, calories, proteins, fats, carbs, "
            "servings, instructions) VALUES "
            "(:title, :ingredients, :tags, :calories, :proteins, :fats, "
            ":carbs, :servings, :instructions)"));
        query.bindValue(QStringLiteral(":title"), title);
        query.bindValue(QStringLiteral(":ingredients"), ingredients);
        query.bindValue(QStringLiteral(":tags"), tags);
        query.bindValue(QStringLiteral(":calories"), calories);
        query.bindValue(QStringLiteral(":proteins"), proteins);
        query.bindValue(QStringLiteral(":fats"), fats);
        query.bindValue(QStringLiteral(":carbs"), carbs);
        query.bindValue(QStringLiteral(":servings"), servings);
        query.bindValue(QStringLiteral(":instructions"), instructions);
        execOrThrow(query);
        return true;
    } catch (const RecipeException &error) {
        qWarning("Не удалось добавить рецепт: %s", error.what());
        return false;
    }
}

bool DatabaseManager::recipeExists(const QString &title)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral("SELECT COUNT(*) FROM recipes WHERE title = :title"));
    query.bindValue(QStringLiteral(":title"), title);
    execOrThrow(query);
    return query.next() && query.value(0).toInt() > 0;
}

bool DatabaseManager::deleteRecipe(int id)
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("DELETE FROM recipes WHERE id = :id"));
        query.bindValue(QStringLiteral(":id"), id);
        execOrThrow(query);
        return true;
    } catch (const RecipeException &error) {
        qWarning("Не удалось удалить рецепт: %s", error.what());
        return false;
    }
}

void DatabaseManager::addToShoppingList(const QString &ingredients)
{
    try {
        QSqlQuery query(m_db);
        const QStringList items = ingredients.split(QLatin1Char(','));
        for (const QString &item : items) {
            const QString clean = item.trimmed().toLower();
            if (clean.isEmpty())
                continue;
            query.prepare(QStringLiteral(
                "INSERT OR IGNORE INTO shopping_list (name) VALUES (:name)"));
            query.bindValue(QStringLiteral(":name"), clean);
            execOrThrow(query);
        }
        emit shoppingListUpdated();
    } catch (const RecipeException &error) {
        qWarning("Ошибка добавления в список покупок: %s", error.what());
    }
}

QStringList DatabaseManager::getShoppingList()
{
    QStringList result;
    try {
        QSqlQuery query(QStringLiteral("SELECT name FROM shopping_list"), m_db);
        execOrThrow(query);
        while (query.next())
            result << query.value(0).toString();
    } catch (const RecipeException &error) {
        qWarning("Ошибка чтения списка покупок: %s", error.what());
    }
    return result;
}

void DatabaseManager::removeFromShoppingList(const QString &name)
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("DELETE FROM shopping_list WHERE name = :name"));
        query.bindValue(QStringLiteral(":name"), name);
        execOrThrow(query);
        emit shoppingListUpdated();
    } catch (const RecipeException &error) {
        qWarning("Ошибка удаления из списка покупок: %s", error.what());
    }
}

void DatabaseManager::clearShoppingList()
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("DELETE FROM shopping_list"));
        execOrThrow(query);
        emit shoppingListUpdated();
    } catch (const RecipeException &error) {
        qWarning("Ошибка очистки списка покупок: %s", error.what());
    }
}

void DatabaseManager::addToFridge(const QString &name)
{
    try {
        const QString clean = name.trimmed().toLower();
        if (clean.isEmpty())
            return;
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral(
            "INSERT OR IGNORE INTO fridge_items (name) VALUES (:name)"));
        query.bindValue(QStringLiteral(":name"), clean);
        execOrThrow(query);
        emit fridgeUpdated();
    } catch (const RecipeException &error) {
        qWarning("Ошибка добавления в холодильник: %s", error.what());
    }
}

QStringList DatabaseManager::getFridgeList()
{
    QStringList result;
    try {
        QSqlQuery query(QStringLiteral("SELECT name FROM fridge_items ORDER BY name"), m_db);
        execOrThrow(query);
        while (query.next())
            result << query.value(0).toString();
    } catch (const RecipeException &error) {
        qWarning("Ошибка чтения холодильника: %s", error.what());
    }
    return result;
}

void DatabaseManager::removeFromFridge(const QString &name)
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("DELETE FROM fridge_items WHERE name = :name"));
        query.bindValue(QStringLiteral(":name"), name);
        execOrThrow(query);
        emit fridgeUpdated();
    } catch (const RecipeException &error) {
        qWarning("Ошибка удаления из холодильника: %s", error.what());
    }
}

void DatabaseManager::clearFridge()
{
    try {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("DELETE FROM fridge_items"));
        execOrThrow(query);
        emit fridgeUpdated();
    } catch (const RecipeException &error) {
        qWarning("Ошибка очистки холодильника: %s", error.what());
    }
}
