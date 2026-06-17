import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: detailsPage
    background: Rectangle { color: window.colorBg }

    // --- Свойства из базы ---
    property string recipeImageUrl: window.selectedImageUrl
    property string recipeTitle: window.selectedTitle
    property string recipeIngredients: window.selectedIngredients
    property string recipeTags: window.selectedTags
    property int recipeCalories: window.selectedCalories
    property double recipeProteins: window.selectedProteins
    property double recipeFats: window.selectedFats
    property double recipeCarbs: window.selectedCarbs
    property int recipeServings: window.selectedServings
    property string recipeInstructions: window.selectedInstructions

    // --- Логика ---
    property int currentServings: recipeServings > 0 ? recipeServings : 1
    property real multiplier: recipeServings > 0 ? currentServings / recipeServings : 1
    property string aiAnalysisResult: "Нажмите 'Запустить анализ', чтобы Шеф изучил состав..."
    property bool isFav: dbManager.isFavorite(recipeTitle)

    // Хранилище для отмеченных галочками ингредиентов (индекс -> true/false)
    property var checkedMap: ({})

    // Сброс состояния при смене рецепта
    onRecipeServingsChanged: {
        currentServings = recipeServings > 0 ? recipeServings : 1
        aiAnalysisResult = "Нажмите 'Запустить анализ', чтобы Шеф изучил состав..."
        checkedMap = {} // Очищаем галочки
    }

    // Функция очистки ингредиента от цифр и единиц измерения (10г -> просто масло)
    function cleanIngredientName(rawName) {
        var name = rawName.trim();
        // 1. Удаляем начальные цифры и дроби (например: "150", "0.5", "1/2")
        name = name.replace(/^[0-9.,\/]+/g, "").trim();
        // 2. Удаляем единицы измерения в начале строки с учетом пробелов и точек
        name = name.replace(/^(г|гр|шт\.?|мл|л|кг|банка|банки|зубчик|зубчика|зубчиков|стакан|стакана|ст\.?\s*л\.?|ч\.?\s*л\.?)\b/gi, "").trim();
        return name;
    }

    ScrollView {
        anchors.fill: parent
        clip: true
        contentWidth: availableWidth

        Column {
            width: Math.min(parent.width - 32, 420)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 25
            topPadding: 10
            bottomPadding: 80

            // 1. ШАПКА С ФОТО
            Rectangle {
                width: parent.width; height: 280; radius: 24; color: "#E8E4DE"; clip: true
                Image {
                    anchors.fill: parent
                    source: detailsPage.recipeImageUrl
                    fillMode: Image.PreserveAspectCrop
                    Rectangle {
                        anchors.fill: parent; color: "#ECE8E1"
                        visible: parent.status !== Image.Ready
                        Label { anchors.centerIn: parent; text: "🥘"; font.pixelSize: 60 }
                    }
                }
                RoundButton {
                    x: 15; y: 15; text: "←"
                    onClicked: tabBar.currentIndex = 0
                    background: Rectangle { radius: 20; color: "white"; opacity: 0.9 }
                }
            }

            // 2. ЗАГОЛОВОК
            Label {
                text: detailsPage.recipeTitle
                font.family: "Georgia"; font.pixelSize: 28; font.bold: true
                color: window.colorTextMain; width: parent.width; wrapMode: Text.WordWrap
            }

            // 3. ПАНЕЛЬ КБЖУ
            RowLayout {
                width: parent.width; spacing: 8
                Repeater {
                    model: [
                        {n: "Ккал", v: (detailsPage.recipeCalories * detailsPage.multiplier).toFixed(0), c: window.colorPrimary},
                        {n: "Белки", v: (detailsPage.recipeProteins * detailsPage.multiplier).toFixed(1), c: window.colorTextMain},
                        {n: "Жиры", v: (detailsPage.recipeFats * detailsPage.multiplier).toFixed(1), c: window.colorTextMain},
                        {n: "Углев.", v: (detailsPage.recipeCarbs * detailsPage.multiplier).toFixed(1), c: "#2D6A4F"}
                    ]
                    delegate: Rectangle {
                        Layout.fillWidth: true; height: 65; radius: 18; color: "white"; border.color: "#F0EBE3"
                        Column {
                            anchors.centerIn: parent; spacing: 2
                            Label { text: modelData.v; font.bold: true; font.pixelSize: 15; color: modelData.c; anchors.horizontalCenter: parent.horizontalCenter }
                            Label { text: modelData.n; font.pixelSize: 10; opacity: 0.4; anchors.horizontalCenter: parent.horizontalCenter }
                        }
                    }
                }
            }

            // 4. ПОРЦИИ
            Rectangle {
                width: parent.width; height: 65; radius: 20; color: "white"; border.color: "#F0EBE3"
                RowLayout {
                    anchors.fill: parent; anchors.margins: 20
                    Label { text: "👥 Порции:"; font.bold: true; color: window.colorTextMain }
                    Item { Layout.fillWidth: true }
                    Row {
                        spacing: 20
                        Button { text: "−"; flat: true; font.bold: true; font.pixelSize: 20; onClicked: if(currentServings > 1) currentServings-- }
                        Label { text: detailsPage.currentServings; font.bold: true; font.pixelSize: 20; color: window.colorPrimary; anchors.verticalCenter: parent.verticalCenter }
                        Button { text: "+"; flat: true; font.bold: true; font.pixelSize: 20; onClicked: currentServings++ }
                    }
                }
            }

            // 5. ИИ-АНАЛИЗ
            Rectangle {
                width: parent.width; height: aiCol.implicitHeight + 40; radius: 24; color: "#F6F8F6"; border.color: "#E2EDE2"
                ColumnLayout {
                    id: aiCol; anchors.fill: parent; anchors.margins: 20; spacing: 15
                    Label { text: "✨ ИИ-АНАЛИЗ ПОЛЬЗЫ"; font.bold: true; font.pixelSize: 11; color: "#2D6A4F" }
                    Text {
                        text: detailsPage.aiAnalysisResult
                        font.pixelSize: 14; font.italic: true; color: window.colorTextMain; wrapMode: Text.WordWrap; Layout.fillWidth: true; lineHeight: 1.2
                    }
                    Button {
                        id: aiBtn
                        text: "Запустить анализ"
                        Layout.fillWidth: true; Layout.preferredHeight: 45
                        background: Rectangle { radius: 12; color: aiBtn.pressed ? "#BF5A39" : window.colorPrimary }
                        contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                        onClicked: detailsPage.aiAnalysisResult = myRecipeService.getAIAnalysis(recipeTitle, recipeTags, recipeCalories)
                    }
                }
            }

            // 6. ИНГРЕДИЕНТЫ (ИСПРАВЛЕНО: Безопасный Column вместо ColumnLayout убирает съезжание)
            Column {
                width: parent.width; spacing: 15

                RowLayout {
                    width: parent.width
                    Label { text: "🥕 Ингредиенты"; font.bold: true; font.pixelSize: 20; color: window.colorTextMain }
                    Item { Layout.fillWidth: true }
                    Button {
                        id: cartBtn
                        text: "+ В КОРЗИНУ"
                        background: Rectangle { radius: 10; color: cartBtn.pressed ? "#BF5A39" : window.colorPrimary }
                        contentItem: Text { text: parent.text; color: "white"; font.bold: true; font.pixelSize: 10; padding: 12 }

                        onClicked: {
                            var ingList = recipeIngredients.split(",");
                            var finalIngredients = [];

                            for (var i = 0; i < ingList.length; i++) {
                                // Если продукт НЕ отмечен галочкой
                                if (!detailsPage.checkedMap[i]) {
                                    // Вызываем наш прокачанный плюсовый метод
                                    var cleaned = myRecipeService.getCleanName(ingList[i]);
                                    if (cleaned.length > 0) {
                                        finalIngredients.push(cleaned);
                                    }
                                }
                            }

                            if (finalIngredients.length > 0) {
                                dbManager.addToShoppingList(finalIngredients.join(", "));
                                console.log("Добавлено в список покупок: " + finalIngredients.join(", "));
                            }
                        }
                    }
                }

                Rectangle {
                    width: parent.width; height: ingCol.height + 40; radius: 24; color: "white"; border.color: "#F0EBE3"
                    Column {
                        id: ingCol; anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top; anchors.margins: 20; spacing: 15
                        Repeater {
                            model: recipeIngredients.split(",")
                            delegate: RowLayout {
                                width: parent.width; spacing: 15
                                Rectangle {
                                    width: 22; height: 22; radius: 6; border.color: "#DDD"; color: mouseArea.checked ? window.colorPrimary : "white"
                                    Label { anchors.centerIn: parent; text: "✓"; visible: mouseArea.checked; color: "white"; font.bold: true }
                                    MouseArea {
                                        id: mouseArea;
                                        anchors.fill: parent;
                                        property bool checked: !!detailsPage.checkedMap[index]
                                        onClicked: {
                                            checked = !checked
                                            // Записываем состояние галочки в общую карту страницы
                                            var map = detailsPage.checkedMap
                                            map[index] = checked
                                            detailsPage.checkedMap = map
                                        }
                                    }
                                }
                                Label {
                                    Layout.fillWidth: true
                                    text: myRecipeService.scaleIngredients(modelData.trim(), detailsPage.multiplier)
                                    font.pixelSize: 15; color: window.colorTextMain; wrapMode: Text.WordWrap
                                }
                            }
                        }
                    }
                }
            }

            // 7. ИНСТРУКЦИЯ (ИСПРАВЛЕНО: Заменен на Column, чтобы разметка шла строго сверху вниз)
            Column {
                width: parent.width; spacing: 12
                Label { text: "👩‍🍳 Инструкция"; font.bold: true; font.pixelSize: 20; color: window.colorTextMain }
                Repeater {
                    model: recipeInstructions.split("\n").filter(s => s.trim().length > 0)
                    delegate: Rectangle {
                        width: parent.width; height: txtStep.implicitHeight + 35; radius: 18; color: "white"; border.color: "#F0EBE3"
                        Label {
                            id: txtStep;
                            anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top; anchors.margins: 18
                            text: modelData.trim(); wrapMode: Text.WordWrap; font.pixelSize: 14; lineHeight: 1.3; color: "#444"
                        }
                    }
                }
            }

            // 8. ДНЕВНИК
            Rectangle {
                width: parent.width; height: 160; radius: 24; color: "white"; border.color: "#F0EBE3"
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 15; spacing: 12
                    Label { text: "🍽️ Записать в дневник на сегодня:"; font.bold: true; Layout.alignment: Qt.AlignHCenter; opacity: 0.6; font.pixelSize: 12 }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 10
                        rowSpacing: 10

                        Repeater {
                            model: ["Завтрак", "Обед", "Ужин", "Перекус"]
                            delegate: Button {
                                id: mBtn
                                Layout.fillWidth: true
                                Layout.preferredHeight: 40
                                text: modelData
                                background: Rectangle { radius: 12; color: mBtn.pressed ? "#BF5A39" : window.colorPrimary }
                                contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                                onClicked: {
                                    window.eatenCalories += detailsPage.recipeCalories * detailsPage.multiplier
                                    window.eatenProteins += detailsPage.recipeProteins * detailsPage.multiplier
                                    window.eatenFats += detailsPage.recipeFats * detailsPage.multiplier
                                    window.eatenCarbs += detailsPage.recipeCarbs * detailsPage.multiplier
                                    if (index === 0) window.breakfastName = recipeTitle
                                    else if (index === 1) window.lunchName = recipeTitle
                                    else if (index === 2) window.dinnerName = recipeTitle
                                    else window.snackName = recipeTitle
                                    console.log("Записано в дневник: " + modelData)
                                }
                            }
                        }
                    }
                }
            }

            // 9. КНОПКА ГОТОВКИ
            Button {
                id: cookBtn
                text: "НАЧАТЬ ГОТОВИТЬ (ПОШАГОВО)"
                width: parent.width; height: 60
                background: Rectangle { color: cookBtn.pressed ? "#BF5A39" : window.colorPrimary; radius: 20 }
                contentItem: Text { text: parent.text; color: "white"; font.bold: true; font.pixelSize: 14; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                onClicked: cookingOverlay.startCooking(recipeInstructions)
            }
        }
    }
}