import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: labPage
    background: Rectangle { color: window.colorBg }

    property string activeTab: "health"
    property bool isSearchActive: false
    property var selectedForSearch: []
    property var targets: ["1931", "65", "45", "180"]


    Connections {
        target: dbManager
        function onFridgeUpdated() { fridgeView.model = dbManager.getFridgeList() }
        function onShoppingListUpdated() { shopView.model = dbManager.getShoppingList() }
    }

    onVisibleChanged: if (visible) {
        fridgeView.model = dbManager.getFridgeList()
        shopView.model = dbManager.getShoppingList()
    }

    function refresh() {
        fridgeView.model = dbManager.getFridgeList()
        shopView.model = dbManager.getShoppingList()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        Rectangle {
            Layout.fillWidth: true; height: 75; color: "white"
            Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: "#F0EBE3" }
            Row {
                anchors.centerIn: parent; spacing: 50
                Repeater {
                    model: [
                        {id: "health", i: "📊", t: "Дневник"},
                        {id: "pantry", i: "🥦", t: "Продукты"},
                        {id: "shop", i: "🛒", t: "Покупки"}
                    ]
                    delegate: Item {
                        width: 60; height: 60
                        Column {
                            anchors.centerIn: parent; spacing: 4
                            Label { text: modelData.i; font.pixelSize: 20; anchors.horizontalCenter: parent.horizontalCenter; opacity: labPage.activeTab === modelData.id ? 1 : 0.4 }
                            Label {
                                text: modelData.t; font.bold: true; font.pixelSize: 9
                                color: labPage.activeTab === modelData.id ? window.colorPrimary : "#BBB"
                            }
                        }
                        Rectangle {
                            width: 4; height: 4; radius: 2; color: window.colorPrimary
                            anchors.bottom: parent.bottom; anchors.bottomMargin: 2; anchors.horizontalCenter: parent.horizontalCenter
                            visible: labPage.activeTab === modelData.id
                        }
                        MouseArea { anchors.fill: parent; onClicked: { labPage.activeTab = modelData.id; labPage.isSearchActive = false } }
                    }
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true; Layout.fillHeight: true; clip: true
            contentWidth: availableWidth

            Column {
                width: Math.min(parent.width - 40, 420)
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 25; topPadding: 20; bottomPadding: 50
                Column {
                    width: parent.width; visible: labPage.activeTab === "health"; spacing: 20
                    Label { text: "Сегодня"; font.family: "Georgia"; font.pixelSize: 28; font.bold: true; color: window.colorTextMain }
                    Rectangle {
                        width: parent.width; height: 240; radius: 30; color: "white"; border.color: "#F0EBE3"
                        Rectangle {
                            anchors.centerIn: parent; width: 160; height: 160; radius: 80; color: "transparent"
                            border.width: 10; border.color: "#FDF5F2"
                            Rectangle {
                                anchors.fill: parent; radius: 80; color: "transparent"
                                border.width: 10; border.color: window.colorPrimary
                            }
                            Column {
                                anchors.centerIn: parent
                                Label {
                                    text: Math.max(0, parseFloat(labPage.targets[0]) - window.eatenCalories).toFixed(0)
                                    font.pixelSize: 34; font.bold: true; color: window.colorTextMain
                                }
                                Label { text: "ккал осталось"; font.pixelSize: 11; color: window.colorTextLight; anchors.horizontalCenter: parent.horizontalCenter }
                            }
                        }
                    }
                    RowLayout {
                        width: parent.width; spacing: 15
                        property var items: [
                            {n: "Белки", v: window.eatenProteins, t: parseFloat(labPage.targets[1]), c: window.colorPrimary},
                            {n: "Жиры", v: window.eatenFats, t: parseFloat(labPage.targets[2]), c: "#E6B89C"},
                            {n: "Углеводы", v: window.eatenCarbs, t: parseFloat(labPage.targets[3]), c: "#8DA399"}
                        ]
                        Repeater {
                            model: parent.items
                            delegate: ColumnLayout {
                                Layout.fillWidth: true; spacing: 6
                                Label { text: modelData.n; font.pixelSize: 10; font.bold: true; color: window.colorTextLight; Layout.alignment: Qt.AlignHCenter }
                                Rectangle {
                                    Layout.fillWidth: true; height: 6; radius: 3; color: "#F0EBE3"
                                    Rectangle {
                                        width: parent.width * Math.min(1.0, (modelData.v / Math.max(1, modelData.t)))
                                        height: 6; radius: 3; color: modelData.c
                                        Behavior on width { NumberAnimation { duration: 500 } }
                                    }
                                }
                                Label { text: Math.round(modelData.v) + "г"; font.pixelSize: 10; font.bold: true; color: window.colorTextMain; Layout.alignment: Qt.AlignHCenter }
                            }
                        }
                    }

                    Label { text: "Питание"; font.family: "Georgia"; font.pixelSize: 20; font.bold: true; color: window.colorTextMain }
                    Column {
                        width: parent.width; spacing: 10
                        Repeater {
                            model: [
                                {t: "Завтрак", n: window.breakfastName, i: "🍳"},
                                {t: "Обед", n: window.lunchName, i: "🍲"},
                                {t: "Ужин", n: window.dinnerName, i: "🌙"},
                                {t: "Перекус", n: window.snackName, i: "🍎"}
                            ]
                            delegate: Rectangle {
                                width: parent.width; height: 75; radius: 22; color: "white"; border.color: "#F0EBE3"
                                RowLayout {
                                    anchors.fill: parent; anchors.margins: 15; spacing: 15
                                    Rectangle { width: 44; height: 44; radius: 22; color: window.colorBg; Label { anchors.centerIn: parent; text: modelData.i; font.pixelSize: 20 } }
                                    Column {
                                        Label { text: modelData.t; font.bold: true; font.pixelSize: 14; color: window.colorTextMain }
                                        Label { text: modelData.n; font.pixelSize: 11; color: window.colorPrimary; elide: Text.ElideRight; width: 180 }
                                    }
                                    Item { Layout.fillWidth: true }
                                    Button { text: "＋"; flat: true; onClicked: tabBar.currentIndex = 0 }
                                }
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width; height: 85; radius: 25; color: "#F2F7F2"; border.color: "#E2EDE2"
                        RowLayout {
                            anchors.fill: parent; anchors.margins: 20
                            Column {
                                spacing: 4
                                Label { text: "🥛 Трекер воды"; font.bold: true; font.pixelSize: 10; color: "#4A5D4C" }
                                Label { text: window.waterCount + " из 8 стаканов"; font.bold: true; font.pixelSize: 16; color: window.colorTextMain }
                            }
                            Item { Layout.fillWidth: true }
                            Button {
                                text: "Добавить"; onClicked: if (window.waterCount < 8) window.waterCount++
                                background: Rectangle { radius: 12; color: "white"; border.color: "#DCE5DC" }
                            }
                        }
                    }

                    Button {
                        text: "ОБНОВИТЬ ПАРАМЕТРЫ ✨"; width: parent.width; height: 55
                        background: Rectangle { color: window.colorTextMain; radius: 15 }
                        contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                        onClicked: calcPopup.open()
                    }
                }
                Column {
                    width: parent.width; visible: labPage.activeTab === "pantry"; spacing: 20
                    Label { text: "Мой Холодильник"; font.family: "Georgia"; font.pixelSize: 26; font.bold: true }
                    RowLayout {
                        width: parent.width; height: 50; spacing: 10
                        TextField {
                            id: fridgeInput; placeholderText: "Добавить продукт..."
                            Layout.fillWidth: true; Layout.preferredHeight: 50
                            background: Rectangle { radius: 12; color: "white"; border.color: fridgeInput.activeFocus ? window.colorPrimary : "#E0DCD5"; border.width: fridgeInput.activeFocus ? 2 : 1 }
                            onAccepted: { if (text.trim() !== "") { dbManager.addToFridge(text.trim()); text = "" } }
                        }
                        Button {
                            text: "＋"; Layout.preferredWidth: 50; Layout.preferredHeight: 50
                            background: Rectangle { radius: 12; color: window.colorPrimary }
                            contentItem: Text { text: parent.text; color: "white"; font.bold: true; font.pixelSize: 18; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                            onClicked: { if (fridgeInput.text.trim() !== "") { dbManager.addToFridge(fridgeInput.text.trim()); fridgeInput.text = "" } }
                        }
                    }

                    ListView {
                        id: fridgeView; width: parent.width; height: contentHeight; spacing: 8; interactive: false
                        delegate: Rectangle {
                            width: fridgeView.width; height: 52; radius: 15; color: "white"; border.color: "#F0EBE3"
                            RowLayout {
                                anchors.fill: parent; anchors.leftMargin: 15; anchors.rightMargin: 10; spacing: 10
                                CheckBox {
                                    id: fridgeBox
                                    text: modelData
                                    Layout.fillWidth: true
                                    checked: labPage.selectedForSearch.indexOf(modelData) !== -1
                                    indicator: Rectangle {
                                        implicitWidth: 22; implicitHeight: 22
                                        x: fridgeBox.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 6
                                        color: fridgeBox.checked ? window.colorPrimary : "transparent"
                                        border.color: fridgeBox.checked ? window.colorPrimary : "#C8C3BC"
                                        border.width: fridgeBox.checked ? 0 : 1.5
                                        Behavior on color { ColorAnimation { duration: 150 } }
                                        Behavior on border.color { ColorAnimation { duration: 150 } }
                                        Text {
                                            text: "✓"; color: "white"; font.bold: true; font.pixelSize: 14
                                            anchors.centerIn: parent; visible: fridgeBox.checked
                                        }
                                    }
                                    contentItem: Text {
                                        text: fridgeBox.text
                                        font.pixelSize: 14; font.weight: Font.Medium
                                        color: fridgeBox.checked ? window.colorTextMain : "#555"
                                        leftPadding: fridgeBox.indicator.width + 12
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    onToggled: {
                                        var list = labPage.selectedForSearch.slice();
                                        if (checked) {
                                            if (list.indexOf(modelData) === -1) list.push(modelData);
                                        } else {
                                            var idx = list.indexOf(modelData);
                                            if (idx !== -1) list.splice(idx, 1);
                                        }
                                        labPage.selectedForSearch = list;
                                    }
                                }
                                Button {
                                    text: "✕"; flat: true; Layout.preferredWidth: 40; Layout.preferredHeight: 40
                                    contentItem: Text { text: parent.text; color: "#CC1111"; opacity: 0.4; font.pixelSize: 14; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                                    onClicked: {
                                        var list = labPage.selectedForSearch.slice();
                                        var idx = list.indexOf(modelData);
                                        if (idx !== -1) { list.splice(idx, 1); labPage.selectedForSearch = list; }
                                        dbManager.removeFromFridge(modelData);
                                    }
                                }
                            }
                        }
                    }

                    Button {
                        text: "НАЙТИ РЕЦЕПТЫ ✨"
                        width: parent.width; height: 55
                        visible: fridgeView.count > 0
                        background: Rectangle { color: labPage.selectedForSearch.length > 0 ? window.colorPrimary : "#DDD"; radius: 15 }
                        contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                        onClicked: {
                            if (labPage.selectedForSearch.length > 0) {
                                pantryModel.findBestRecipes(labPage.selectedForSearch.join(","))
                                labPage.isSearchActive = true
                            }
                        }
                    }

                    Column {
                        width: parent.width; spacing: 15; visible: labPage.isSearchActive
                        Label { text: "РЕЗУЛЬТАТЫ ПО УБЫВАНИЮ СОВПАДЕНИЙ:"; font.bold: true; font.pixelSize: 11; color: window.colorPrimary }
                        ListView {
                            width: parent.width; height: contentHeight; spacing: 12; interactive: false
                            model: pantryModel
                            delegate: RecipeCard { currentModel: pantryModel }
                        }
                    }
                }

                Column {
                    width: parent.width
                    visible: labPage.activeTab === "shop"
                    spacing: 20
                    Label {
                        text: "Список покупок"
                        font.family: "Georgia"; font.pixelSize: 26; font.bold: true; color: window.colorTextMain
                    }

                    RowLayout {
                        width: parent.width; height: 50; spacing: 10
                        TextField {
                            id: shopInput
                            placeholderText: "Нужно купить..."
                            Layout.fillWidth: true; Layout.preferredHeight: 50
                            font.pixelSize: 14; leftPadding: 15
                            background: Rectangle {
                                radius: 12; color: "white"
                                border.color: shopInput.activeFocus ? window.colorPrimary : "#E0DCD5"
                                border.width: shopInput.activeFocus ? 2 : 1
                            }
                            onAccepted: {
                                if (text.trim() !== "") {
                                    dbManager.addToShoppingList(text.trim())
                                    text = ""
                                    refresh()
                                }
                            }
                        }
                        Button {
                            text: "＋"
                            Layout.preferredWidth: 55; Layout.preferredHeight: 50
                            background: Rectangle { radius: 12; color: window.colorPrimary }
                            contentItem: Text { text: "+"; color: "white"; font.bold: true; font.pixelSize: 22; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                            onClicked: {
                                if (shopInput.text.trim() !== "") {
                                    dbManager.addToShoppingList(shopInput.text.trim())
                                    shopInput.text = ""
                                    refresh()
                                }
                            }
                        }
                    }

                    ListView {
                        id: shopView
                        width: parent.width; height: contentHeight
                        spacing: 10; interactive: false
                        model: dbManager.getShoppingList()
                        delegate: Rectangle {
                            width: shopView.width; height: 54; radius: 15
                            color: "white"; border.color: "#F0EBE3"
                            RowLayout {
                                anchors.fill: parent; anchors.leftMargin: 15; anchors.rightMargin: 10; spacing: 12
                                CheckBox {
                                    id: shopBox
                                    text: modelData
                                    Layout.fillWidth: true; Layout.alignment: Qt.AlignVCenter
                                    indicator: Rectangle {
                                        implicitWidth: 24; implicitHeight: 24; radius: 7
                                        y: parent.height / 2 - height / 2
                                        color: shopBox.checked ? window.colorPrimary : "transparent"
                                        border.color: shopBox.checked ? window.colorPrimary : "#C8C3BC"
                                        border.width: shopBox.checked ? 0 : 1.5
                                        Text { text: "✓"; color: "white"; font.bold: true; anchors.centerIn: parent; visible: shopBox.checked }
                                    }
                                    contentItem: Text {
                                        text: shopBox.text
                                        font.pixelSize: 15
                                        color: shopBox.checked ? "#AAA" : window.colorTextMain
                                        leftPadding: 38; verticalAlignment: Text.AlignVCenter
                                        font.strikeout: shopBox.checked
                                        opacity: shopBox.checked ? 0.6 : 1.0
                                    }
                                    onToggled: {
                                        if (checked) {
                                            dbManager.addToFridge(modelData)
                                            dbManager.removeFromShoppingList(modelData)
                                            refresh()
                                        }
                                    }
                                }
                                Button {
                                    text: "✕"; flat: true
                                    Layout.preferredWidth: 40; Layout.preferredHeight: 40; Layout.alignment: Qt.AlignVCenter
                                    contentItem: Text { text: "✕"; color: "#CC1111"; opacity: 0.2; font.pixelSize: 16; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                                    onClicked: { dbManager.removeFromShoppingList(modelData); refresh() }
                                }
                            }
                        }
                    }

                    Button {
                        text: "ОЧИСТИТЬ КОРЗИНУ"
                        width: parent.width; flat: true
                        onClicked: { dbManager.clearShoppingList(); refresh() }
                        contentItem: Text {
                            text: parent.text; font.pixelSize: 11; font.bold: true; font.underline: true
                            color: window.colorTextLight; horizontalAlignment: Text.AlignHCenter; opacity: 0.6
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: calcPopup
        x: (parent.width - width) / 2; y: 50; width: 340; modal: true
        background: Rectangle { radius: 24; color: "white"; border.color: "#EEE" }
        Column {
            anchors.fill: parent; anchors.margins: 25; spacing: 15
            Label { text: "Ваши параметры"; font.family: "Georgia"; font.pixelSize: 22; font.bold: true }
            TextField { id: weightIn; placeholderText: "Вес (кг)"; width: parent.width }
            TextField { id: heightIn; placeholderText: "Рост (см)"; width: parent.width }
            TextField { id: ageIn; placeholderText: "Возраст"; width: parent.width }
            Button {
                text: "РАССЧИТАТЬ"; width: parent.width; height: 50
                background: Rectangle { color: window.colorTextMain; radius: 12 }
                contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                onClicked: {
                    let d = myRecipeService.calculateFullNutrients(parseFloat(weightIn.text), parseFloat(heightIn.text), parseInt(ageIn.text), true, 1.2)
                    labPage.targets = d.split("|")
                    window.eatenCalories = 0
                    calcPopup.close()
                }
            }
        }
    }
}
