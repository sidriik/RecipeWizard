import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    background: Rectangle { color: style.bg }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Label {
            text: "Набор ингридиентов"
            font.pixelSize: 22; font.bold: true; color: style.text
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            color: "white"; radius: style.radius; border.color: style.accent

            GridLayout {
                anchors.fill: parent; anchors.margins: 15
                columns: 3
                Repeater {
                    model: ["яйца", "сыр", "курица", "молоко", "помидоры", "рис", "творог", "банан"]
                    CheckBox {
                        id: cb
                        text: modelData
                        property string val: modelData
                    }
                }
            }
        }

        Button {
            Layout.fillWidth: true
            text: "Придумать рецепт ✨"
            background: Rectangle { color: style.primary; radius: 30 }
            contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter; font.bold: true }

            onClicked: {
                let selected = []
                myRecipeModel.findBestRecipes("яйца,сыр")
                tabBar.currentIndex = 0
            }
        }
    }
}
