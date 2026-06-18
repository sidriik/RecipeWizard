import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property var currentModel: mainModel

    width: ListView.view.width
    height: 140
    radius: 16
    color: "white"
    border.color: Qt.rgba(58 / 255, 64 / 255, 50 / 255, 0.1)
    border.width: 1

    property bool hovered: false
    Behavior on border.color { ColorAnimation { duration: 300 } }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: root.hovered = true
        onExited: root.hovered = false
        onClicked: {
            window.selectedTitle = model.title
            window.selectedImageUrl = model.imageUrl
            window.selectedIngredients = model.ingredients
            window.selectedTags = model.tags
            window.selectedCalories = model.calories
            window.selectedProteins = model.proteins
            window.selectedFats = model.fats
            window.selectedCarbs = model.carbs
            window.selectedServings = model.servings
            window.selectedInstructions = model.instructions || ""
            tabBar.currentIndex = 4
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16
        Item {
            Layout.preferredWidth: 96
            Layout.preferredHeight: 96

            Rectangle {
                id: imageCircle
                anchors.fill: parent
                radius: 48
                color: "#FBF9F6"
                border.color: Qt.rgba(58 / 255, 64 / 255, 50 / 255, 0.05)
                clip: true

                Image {
                    id: recipeImg
                    anchors.fill: parent
                    source: model.imageUrl || ""
                    fillMode: Image.PreserveAspectCrop
                    asynchronous: true
                    opacity: status === Image.Ready ? 1 : 0
                    Behavior on opacity { NumberAnimation { duration: 300 } }
                }

                Label {
                    anchors.centerIn: parent
                    visible: recipeImg.status !== Image.Ready
                    text: "🥘"
                    font.pixelSize: 40
                }

                Rectangle {
                    width: 28; height: 28; radius: 14
                    anchors.right: parent.right; anchors.bottom: parent.bottom
                    color: "white"
                    border.color: Qt.rgba(58 / 255, 64 / 255, 50 / 255, 0.05)
                    Label {
                        anchors.centerIn: parent
                        text: dbManager.isFavorite(model.title) ? "❤️" : "🤍"
                        font.pixelSize: 12
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            currentModel.toggleFavoriteInModel(index)

                            if (typeof favoritesModel !== "undefined")
                                favoritesModel.refresh()
                        }
                    }
                }
            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Label {
                text: model.title
                font.family: "Georgia"; font.pixelSize: 18; font.bold: true
                color: root.hovered ? "#D96C4A" : "#3A4032"
                Layout.fillWidth: true
                elide: Text.ElideRight; maximumLineCount: 2; wrapMode: Text.WordWrap
            }
            Text {
                text: "🌱 <b>Состав:</b> " + model.ingredients
                font.pixelSize: 11
                color: Qt.rgba(58 / 255, 64 / 255, 50 / 255, 0.6)
                Layout.fillWidth: true
                elide: Text.ElideRight; maximumLineCount: 2; wrapMode: Text.WordWrap
                lineHeight: 1.2
            }
            Rectangle {
                Layout.fillWidth: true; height: 1
                color: Qt.rgba(58 / 255, 64 / 255, 50 / 255, 0.05)
                Layout.topMargin: 4
            }
            RowLayout {
                Layout.fillWidth: true
                Text {
                    text: "<b>КБЖУ:</b> "
                          + model.calories + " ккал  "
                          + "Б" + model.proteins + "  "
                          + "Ж" + model.fats + "  "
                          + "У" + model.carbs
                    font.pixelSize: 10; color: "#D96C4A"
                }
                Item { Layout.fillWidth: true }
                Rectangle {
                    height: 22; width: servingsLabel.implicitWidth + 16; radius: 11
                    color: Qt.rgba(194 / 255, 213 / 255, 196 / 255, 0.3)
                    Label {
                        id: servingsLabel
                        anchors.centerIn: parent
                        text: model.servings + " порции"
                        font.pixelSize: 10; font.bold: true; color: "#3A4032"
                    }
                }
                Label {
                    text: "🌱".repeat(Math.min(3, Math.max(1, model.id % 3 + 1)))
                    font.family: "Monospace"; color: "#A8A29E"
                }
            }
        }
    }
}
