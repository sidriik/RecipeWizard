import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    property var history: []

    function refreshHistory() {
        history = dbManager.getHistory()
    }

    ListView {
        id: historyList
        anchors.fill: parent
        anchors.margins: 16
        spacing: 10
        model: history
        clip: true

        delegate: Rectangle {
            width: historyList.width
            height: 70
            color: "#e8f5e9"
            radius: 8

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                Label {
                    text: "📖"
                    font.pixelSize: 22
                }

                ColumnLayout {
                    Layout.fillWidth: true

                    Label {
                        text: modelData
                        font.bold: true
                        font.pixelSize: 16
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    Label {
                        text: "Недавно просмотренный рецепт"
                        color: "gray"
                        font.pixelSize: 12
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }

    Component.onCompleted: refreshHistory()

    onVisibleChanged: {
        if (visible) refreshHistory()
    }
}