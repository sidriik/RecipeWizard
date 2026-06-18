import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "pages"

ApplicationWindow {
    id: window
    visible: true
    width: 380
    height: 700
    title: "RecipeWizard"
    property color colorBg: "#FDFBF7"
    property color colorPrimary: "#D87D56"
    property color colorSecondary: "#EAF1EA"
    property color colorTextMain: "#3A4032"
    property color colorTextLight: "#7A8074"
    property int globalRadius: 24
    property string selectedTitle: ""
    property string selectedImageUrl: ""
    property string selectedIngredients: ""
    property string selectedTags: ""
    property var selectedCalories: 0
    property var selectedProteins: 0
    property var selectedFats: 0
    property var selectedCarbs: 0
    property var selectedServings: 0
    property string selectedInstructions: ""
    property real eatenCalories: 0
    property real eatenProteins: 0
    property real eatenFats: 0
    property real eatenCarbs: 0
    property int waterCount: 0

    property string breakfastName: "Пока пусто"
    property string lunchName: "Пока пусто"
    property string dinnerName: "Пока пусто"
    property string snackName: "Пока пусто"

    footer: TabBar {
        id: tabBar
        currentIndex: 0
        height: 55
        background: Rectangle {
            color: "white"
            Rectangle { anchors.top: parent.top; width: parent.width; height: 1; color: "#F0EBE3" }
        }

        Repeater {
            model: [
                {i: "🏠", n: "Главная"},
                {i: "👨‍🍳", n: "Продукты"},
                {i: "⭐", n: "Любимое"},
                {i: "➕", n: "Создать"}
            ]
            delegate: TabButton {
                width: window.width / 4
                height: 55
                contentItem: Column {
                    anchors.centerIn: parent
                    spacing: 2
                    Label {
                        text: modelData.i; font.pixelSize: 18
                        anchors.horizontalCenter: parent.horizontalCenter
                        opacity: checked ? 1.0 : 0.5
                    }
                    Label {
                        text: modelData.n; font.pixelSize: 8; font.bold: true
                        color: checked ? window.colorPrimary : "#A0A0A0"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
                background: Rectangle {
                    color: "transparent"
                    Rectangle {
                        anchors.bottom: parent.bottom; anchors.bottomMargin: 4
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 4; height: 4; radius: 2
                        color: window.colorPrimary; visible: checked
                    }
                }
            }
        }
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        HomePage {}
        LaboratoryPage {}
        FavoritesPage {}
        AddRecipePage {}
        RecipeDetailsPage { id: detailsPage }
    }

    CookingOverlay { id: cookingOverlay; anchors.fill: parent; z: 1000 }
}
