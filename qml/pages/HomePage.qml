import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: homePage
    property var currentModel: mainModel
    property int calorieIndex: 2

    ListModel {
        id: tagsModel
        ListElement { name: "обед"; selected: false }
        ListElement { name: "ужин"; selected: false }
        ListElement { name: "завтрак"; selected: false }
        ListElement { name: "перекус"; selected: false }
        ListElement { name: "без сахара"; selected: false }
        ListElement { name: "без глютена"; selected: false }
        ListElement { name: "высокобелковое"; selected: false }
        ListElement { name: "веган"; selected: false }
        ListElement { name: "кето"; selected: false }
        ListElement { name: "быстро"; selected: false }
    }

    function applyTagFilter() {
        let list = []
        for (let i = 0; i < tagsModel.count; ++i) {
            if (tagsModel.get(i).selected) {
                list.push(tagsModel.get(i).name)
            }
        }
        let calLimit = [300, 600, 2000][homePage.calorieIndex]
        mainModel.applyFilters(list, calLimit)
    }

    background: Rectangle { color: window.colorBg }
    readonly property var seasonData: {
        let month = new Date().getMonth();
        if (month >= 2 && month <= 4)
            return { name: "ВЕСНА", icon: "🌞🐝🌸🪴🐌", tag: "весна", color: "#8DA399",
                     desc: "Время обновлений: молодая зелень, редис и лёгкие весенние супы." };
        if (month >= 5 && month <= 7)
            return { name: "ЛЕТО", icon: "🏝️🍹⛱️🌞 🌊", tag: "лето", color: window.colorPrimary,
                     desc: "Освежающие салаты и холодный клубничный мохито. Наслаждайтесь прохладой!" };
        if (month >= 8 && month <= 10)
            return { name: "ОСЕНЬ", icon: "🍂🧸🥐☕🍪", tag: "осень", color: "#AF6E4D",
                     desc: "Уютные тыквенные супы, пряный имбирь и согревающие ароматы корицы." };
        return { name: "ЗИМА", icon: "❄️🧣☃️🍫☕️", tag: "зима", color: "#5D8AA8",
                 desc: "Сытные запеканки, горячий шоколад и витаминные цитрусовые рецепты." };
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        clip: true

        Column {
            width: parent.width - 32
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 25
            topPadding: 10
            bottomPadding: 30

            Rectangle {
                width: parent.width; height: 170
                radius: window.globalRadius; color: "#ECE8E1"
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 20
                    Label {
                        text: homePage.seasonData.icon + " СЕЗОННЫЙ РИТМ: " + homePage.seasonData.name
                        font.pixelSize: 12; font.bold: true
                        color: homePage.seasonData.color
                        font.letterSpacing: 1.5
                    }
                    Label {
                        text: "Осознанная Кухня"
                        font.pixelSize: 24; font.family: "Georgia"; color: window.colorTextMain
                    }
                    Text {
                        text: "\"" + homePage.seasonData.desc + "\""
                        font.pixelSize: 14; color: window.colorTextLight; font.italic: true
                        Layout.fillWidth: true; wrapMode: Text.WordWrap
                        lineHeight: 1.2
                    }
                }
                Button {
                    anchors.right: parent.right; anchors.bottom: parent.bottom; anchors.margins: 16
                    text: "+ Свой рецепт"
                    background: Rectangle { color: window.colorPrimary; radius: 25 }
                    contentItem: Text { text: parent.text; color: "white"; font.bold: true; padding: 12 }
                    onClicked: tabBar.currentIndex = 3
                }
            }

            Rectangle {
                width: parent.width; height: 58; radius: 29; color: "white"; border.color: "#E8E4DE"
                RowLayout {
                    anchors.fill: parent; anchors.margins: 6; spacing: 10
                    Label { text: "👀"; Layout.leftMargin: 15; opacity: 0.4 }
                    TextField {
                        id: searchField
                        placeholderText: "Найти блюдо..."
                        Layout.fillWidth: true; background: null
                        font.pixelSize: 14; color: window.colorTextMain
                        onTextChanged: mainModel.searchByTitle(text)
                    }
                    Button {
                        visible: searchField.text !== ""
                        flat: true; text: "✕"
                        onClicked: { searchField.text = ""; mainModel.refresh() }
                    }
                }
            }

            Flickable {
                width: parent.width; height: 50; contentWidth: tagsRow.width; clip: true
                flickableDirection: Flickable.HorizontalFlick
                Row {
                    id: tagsRow; spacing: 10; anchors.verticalCenter: parent.verticalCenter
                    Button {
                        id: allBtn
                        text: "⋆☕︎ ˖️ Все меню"
                        property bool nothingSelected: {
                            for (let i = 0; i < tagsModel.count; i++) if (tagsModel.get(i).selected) return false
                            return true
                        }
                        background: Rectangle { radius: 20; color: allBtn.nothingSelected ? window.colorPrimary : "#F0F4EF" }
                        contentItem: Text { text: parent.text; color: allBtn.nothingSelected ? "white" : window.colorTextMain; font.bold: true; padding: 15 }
                        onClicked: {
                            for (let i = 0; i < tagsModel.count; ++i) tagsModel.setProperty(i, "selected", false)
                            homePage.calorieIndex = 2
                            mainModel.applyFilters([], 2000)
                        }
                    }
                    Repeater {
                        model: tagsModel
                        delegate: Button {
                            text: "#" + model.name
                            background: Rectangle { radius: 20; color: model.selected ? window.colorPrimary : "#F0F4EF" }
                            contentItem: Text { text: parent.text; color: model.selected ? "white" : window.colorTextMain; font.bold: true; padding: 15 }
                            onClicked: {
                                tagsModel.setProperty(index, "selected", !model.selected)
                                homePage.applyTagFilter()
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width; height: 90; radius: window.globalRadius; color: "white"; border.color: "#F0EBE3"
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 15; spacing: 8
                    Label { text: "♨ Энергетическая ценность"; font.bold: true; font.pixelSize: 13; color: window.colorTextMain }
                    Rectangle {
                        Layout.fillWidth: true; Layout.preferredHeight: 40; radius: 12; color: "#F5F2EE"
                        Rectangle {
                            id: selectionBar
                            width: parent.width / 3 - 4; height: parent.height - 8; radius: 10; color: "white"
                            border.color: window.colorPrimary; border.width: 1
                            x: 4 + (parent.width / 3) * homePage.calorieIndex; y: 4
                            Behavior on x { NumberAnimation { duration: 250; easing.type: Easing.OutCubic } }
                        }
                        Row {
                            anchors.fill: parent
                            Repeater {
                                model: ["Легко", "Сытно", "Все"]
                                delegate: MouseArea {
                                    width: parent.width / 3; height: parent.height
                                    Label {
                                        anchors.centerIn: parent; text: modelData; font.pixelSize: 12
                                        color: homePage.calorieIndex === index ? window.colorPrimary : window.colorTextLight
                                    }
                                    onClicked: { homePage.calorieIndex = index; homePage.applyTagFilter() }
                                }
                            }
                        }
                    }
                }
            }

            Column {
                width: parent.width; spacing: 12
                Label { text: "⋆˚✿˖° СВЕЖИЙ ВЫБОР СЕЗОНА "; font.bold: true; color: window.colorTextMain; font.pixelSize: 13 }
                Flickable {
                    width: parent.width; height: 210; contentWidth: seasonRow.width; clip: true
                    Row {
                        id: seasonRow; spacing: 15
                        Repeater {
                            model: Math.min(3, recipeList.count)
                            delegate: Rectangle {
                                width: 180; height: 200; radius: 20; color: "white"; border.color: "#F0EBE3"; clip: true
                                Column {
                                    anchors.fill: parent; anchors.margins: 12; spacing: 8
                                    Rectangle {
                                        width: parent.width; height: 100; radius: 15; color: window.colorBg; clip: true
                                        Image {
                                            anchors.fill: parent; source: mainModel.getVal(index, "imageUrl") || ""; fillMode: Image.PreserveAspectCrop
                                            Rectangle { anchors.fill: parent; color: "#ECE8E1"; visible: parent.status !== Image.Ready; Label { anchors.centerIn: parent; text: "🥘"; font.pixelSize: 30 } }
                                        }
                                        Rectangle {
                                            anchors.right: parent.right; anchors.bottom: parent.bottom; anchors.margins: 8
                                            width: 50; height: 22; radius: 8; color: "white"
                                            Label { anchors.centerIn: parent; text: "15 мин"; font.pixelSize: 9; font.bold: true }
                                        }
                                    }
                                    Label { text: mainModel.getVal(index, "title") || ""; font.bold: true; font.pixelSize: 14; width: parent.width; elide: Text.ElideRight }
                                    Label { text: (mainModel.getVal(index, "calories") || "0") + " ккал"; color: window.colorPrimary; font.pixelSize: 11 }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        window.selectedTitle = mainModel.getVal(index, "title"); window.selectedImageUrl = mainModel.getVal(index, "imageUrl")
                                        window.selectedIngredients = mainModel.getVal(index, "ingredients"); window.selectedTags = mainModel.getVal(index, "tags")
                                        window.selectedCalories = mainModel.getVal(index, "calories"); window.selectedProteins = mainModel.getVal(index, "proteins")
                                        window.selectedFats = mainModel.getVal(index, "fats"); window.selectedCarbs = mainModel.getVal(index, "carbs")
                                        window.selectedServings = mainModel.getVal(index, "servings"); window.selectedInstructions = mainModel.getVal(index, "instructions")
                                        tabBar.currentIndex = 4
                                    }
                                }
                            }
                        }
                    }
                }
            }

            RowLayout {
                width: parent.width
                ColumnLayout {
                    spacing: 2
                    Label { text: "ВЫБОР БЛЮД"; font.bold: true; color: window.colorTextMain; font.pixelSize: 16 }
                    Label {
                        text: recipeList.count + " рецептов найдено"
                        color: window.colorTextLight; font.pixelSize: 11
                    }
                }
                Item { Layout.fillWidth: true }
                Button {
                    flat: true; contentItem: Label { text: "ಠ Сбросить всё"; font.bold: true; color: window.colorPrimary; font.pixelSize: 12 }
                    onClicked: { searchField.text = ""; for (let i = 0; i < tagsModel.count; ++i) tagsModel.setProperty(i, "selected", false); homePage.calorieIndex = 2; mainModel.refresh() }
                }
            }

            ListView {
                id: recipeList
                model: mainModel
                width: parent.width; height: contentHeight
                spacing: 12; interactive: false
                delegate: RecipeCard { currentModel: mainModel }
            }
        }
    }
}
