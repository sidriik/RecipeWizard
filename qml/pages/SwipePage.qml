import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: swipePage

    // Фон используем из нашего объекта style в main.qml
    background: Rectangle { color: style.bg }

    property int currentIndex: 0

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // Заголовок страницы
        Label {
            text: "Фуд-Свайп 🔥"
            font.pixelSize: 24
            font.bold: true
            color: style.text
            Layout.alignment: Qt.AlignHCenter
        }

        // КАРТОЧКА БЛЮДА (Центральный элемент)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"
            radius: style.radius
            border.color: style.accent
            border.width: 1

            // Чтобы контент не вылезал за скругленные углы
            clip: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                // Место под "картинку" (в нашем случае иконка/эмодзи)
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 180
                    color: style.bg
                    radius: 15
                    Label {
                        anchors.centerIn: parent
                        text: "🍽️"
                        font.pixelSize: 60
                    }
                }

                // НАЗВАНИЕ (используем твой новый метод getVal)
                Label {
                    text: myRecipeModel.getVal(swipePage.currentIndex, "title")
                    font.pixelSize: 22
                    font.bold: true
                    color: style.text
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                // КАЛОРИИ
                Label {
                    text: "🔥 " + myRecipeModel.getVal(swipePage.currentIndex, "calories") + " ккал"
                    color: style.primary
                    font.bold: true
                    font.pixelSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }

                // ИНСТРУКЦИЯ (короткое превью)
                Text {
                    text: myRecipeModel.getVal(swipePage.currentIndex, "instructions")
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    color: "#555"
                    font.pixelSize: 14
                    maximumLineCount: 4
                    elide: Text.ElideRight
                    lineHeight: 1.2
                }

                Item { Layout.fillHeight: true } // Распорка, чтобы прижать текст к верху
            }
        }

        // КНОПКИ УПРАВЛЕНИЯ
        RowLayout {
            Layout.fillWidth: true
            height: 60
            spacing: 15

            // Кнопка "Пропустить"
            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                text: "❌ Пропустить"

                onClicked: {
                    // Просто переходим к следующему индексу
                    if (myRecipeModel.rowCount() > 0) {
                        swipePage.currentIndex = (swipePage.currentIndex + 1) % myRecipeModel.rowCount()
                    }
                }
            }

            // Кнопка "Нравится"
            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                text: "❤️ Нравится"

                // Стилизуем кнопку под наш дизайн
                background: Rectangle {
                    color: style.primary
                    radius: 25
                }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (myRecipeModel.rowCount() > 0) {
                        // Берем название через getVal и добавляем в избранное
                        let currentTitle = myRecipeModel.getVal(swipePage.currentIndex, "title")
                        dbManager.addFavorite(currentTitle)

                        // Идем к следующему
                        swipePage.currentIndex = (swipePage.currentIndex + 1) % myRecipeModel.rowCount()
                    }
                }
            }
        }
    }
}