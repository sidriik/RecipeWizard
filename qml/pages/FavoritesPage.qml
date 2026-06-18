import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: favoritesPage
    background: Rectangle { color: window.colorBg }

    onVisibleChanged: if (visible) favoritesView.forceLayout()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Label {
            text: "❤️ Любимые рецепты"
            font.family: "Georgia"; font.pixelSize: 28; font.bold: true
            color: window.colorTextMain
            Layout.fillWidth: true; Layout.topMargin: 10
        }

        ListView {
            id: favoritesView
            Layout.fillWidth: true; Layout.fillHeight: true
            clip: true
            model: mainModel
            spacing: 0

            delegate: Item {
                id: delegateWrapper
                width: favoritesView.width
                property bool isFavorite: dbManager.isFavorite(model.title)
                height: isFavorite ? 152 : 0
                visible: isFavorite

                Behavior on height {
                    NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
                }

                RecipeCard {
                    width: parent.width
                    height: 140
                    anchors.top: parent.top
                }
            }
        }
    }
}
