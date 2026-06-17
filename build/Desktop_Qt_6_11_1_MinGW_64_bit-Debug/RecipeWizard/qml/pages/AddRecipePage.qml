import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: addPage
    background: Rectangle { color: window.colorBg }

    ScrollView {
        anchors.fill: parent
        clip: true
        contentWidth: availableWidth

        Column {
            // Ограничиваем ширину и центрируем
            width: Math.min(parent.width - 40, 450)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            topPadding: 20
            bottomPadding: 50

            Label {
                text: "Новый рецепт"
                font.family: "Georgia"; font.pixelSize: 28; font.bold: true; color: window.colorTextMain
            }

            // --- КАРТОЧКА 1: ОСНОВНОЕ ---
            Rectangle {
                width: parent.width; height: contentInfo.implicitHeight + 40
                radius: 20; color: "white"; border.color: "#F0EBE3"

                ColumnLayout {
                    id: contentInfo; anchors.fill: parent; anchors.margins: 20; spacing: 15

                    TextField {
                        id: titleField; placeholderText: "Название блюда"; Layout.fillWidth: true
                        background: Rectangle { radius: 10; border.color: "#EEE" }
                    }
                    TextField {
                        id: tagsField; placeholderText: "Теги (через запятую: обед, завтрак...)"; Layout.fillWidth: true
                        background: Rectangle { radius: 10; border.color: "#EEE" }
                    }
                }
            }

            // --- КАРТОЧКА 2: ИНГРЕДИЕНТЫ ---
            Label { text: "Ингредиенты"; font.bold: true; font.pixelSize: 14; opacity: 0.6 }
            Rectangle {
                width: parent.width; height: 120; radius: 20; color: "white"; border.color: "#F0EBE3"
                TextArea {
                    id: ingredientsField
                    anchors.fill: parent; anchors.margins: 15
                    placeholderText: "Пример: 2 шт. яйца, 100г сыр..."
                    wrapMode: TextEdit.Wrap; font.pixelSize: 14
                }
            }

            // --- КАРТОЧКА 3: КБЖУ ---
            Label { text: "Пищевая ценность (на порцию)"; font.bold: true; font.pixelSize: 14; opacity: 0.6 }
            Rectangle {
                width: parent.width; height: 130; radius: 20; color: "white"; border.color: "#F0EBE3"
                GridLayout {
                    anchors.fill: parent; anchors.margins: 20; columns: 2; rowSpacing: 10; columnSpacing: 20
                    TextField { id: caloriesField; placeholderText: "Ккал"; Layout.fillWidth: true }
                    TextField { id: proteinsField; placeholderText: "Белки (г)"; Layout.fillWidth: true }
                    TextField { id: fatsField; placeholderText: "Жиры (г)"; Layout.fillWidth: true }
                    TextField { id: carbsField; placeholderText: "Углеводы (г)"; Layout.fillWidth: true }
                }
            }

            // --- КАРТОЧКА 4: ШАГИ ---
            Label { text: "Инструкция (каждый шаг с новой строки)"; font.bold: true; font.pixelSize: 14; opacity: 0.6 }
            Rectangle {
                width: parent.width; height: 150; radius: 20; color: "white"; border.color: "#F0EBE3"
                TextArea {
                    id: instructionsField
                    anchors.fill: parent; anchors.margins: 15
                    placeholderText: "1. Разбить яйца...\n2. Пожарить..."
                    wrapMode: TextEdit.Wrap; font.pixelSize: 14
                }
            }

            // КНОПКА СОХРАНЕНИЯ
            Button {
                text: "СОХРАНИТЬ РЕЦЕПТ В БАЗУ ✨"
                width: parent.width; height: 60

                background: Rectangle {
                    color: window.colorPrimary
                    radius: 15
                }
                contentItem: Text {
                    text: parent.text; color: "white"; font.bold: true
                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (titleField.text !== "") {
                        dbManager.addRecipe(
                            titleField.text, ingredientsField.text, tagsField.text,
                            parseInt(caloriesField.text) || 0,
                            parseFloat(proteinsField.text) || 0,
                            parseFloat(fatsField.text) || 0,
                            parseFloat(carbsField.text) || 0,
                            1, // порции по умолчанию
                            instructionsField.text
                        )
                        myRecipeModel.refresh()

                        // Очистка
                        titleField.text = ""; ingredientsField.text = ""; tagsField.text = ""
                        caloriesField.text = ""; proteinsField.text = ""; fatsField.text = ""; carbsField.text = ""
                        instructionsField.text = ""

                        tabBar.currentIndex = 0 // На главную
                    }
                }
            }
        }
    }
}