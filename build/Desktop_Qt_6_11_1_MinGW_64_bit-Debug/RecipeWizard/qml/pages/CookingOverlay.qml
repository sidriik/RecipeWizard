import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    anchors.fill: parent
    color: window.colorBg
    visible: false
    z: 2000 // Самый высокий приоритет, чтобы перекрыть всё

    property var steps: []
    property int currentStep: 0
    property int remainingSeconds: 0
    property bool isTimerRunning: false

    // Запуск процесса
    function startCooking(instructions) {
        if (!instructions) return;
        steps = myRecipeService.getStepList(instructions);
        currentStep = 0;
        updateStep();
        visible = true;
    }

    // Обновление данных при смене шага
    function updateStep() {
        isTimerRunning = false;
        remainingSeconds = myRecipeService.getTimerSeconds(steps[currentStep]);
    }

    // Объект таймера (реальный отсчет)
    Timer {
        id: countdown
        interval: 1000; repeat: true; running: isTimerRunning
        onTriggered: {
            if (remainingSeconds > 0) remainingSeconds--;
            else isTimerRunning = false;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 20

        // Кнопка закрыть
        RowLayout {
            Layout.fillWidth: true
            Label { text: "ШАГ " + (currentStep + 1) + " ИЗ " + steps.length; font.bold: true; opacity: 0.5 }
            Item { Layout.fillWidth: true }
            Button { text: "✕"; flat: true; onClicked: { isTimerRunning = false; root.visible = false } }
        }

        // Текст инструкции (Georgia)
        Label {
            Layout.fillWidth: true; Layout.fillHeight: true
            text: steps.length > 0 ? steps[currentStep] : ""
            font.family: "Georgia"; font.pixelSize: 28; color: window.colorTextMain
            horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap; lineHeight: 1.3
        }

        // Блок ТАЙМЕРА (виден только если в тексте есть время)
        // --- ОБНОВЛЕННЫЙ СТИЛЬНЫЙ ТАЙМЕР ---
        Rectangle {
            id: timerCircle
            visible: remainingSeconds > 0
            Layout.alignment: Qt.AlignHCenter
            width: 220; height: 220; radius: 110
            color: "white"

            // Мягкая тень вокруг таймера
            border.color: "#F0EBE3"
            border.width: 1

            // Фоновое кольцо (шкала)
            Rectangle {
                anchors.centerIn: parent
                width: 190; height: 190; radius: 95
                color: "transparent"
                border.width: 8
                border.color: "#FDF5F2" // Очень светлый терракот
            }

            // Активное кольцо прогресса
            Rectangle {
                anchors.centerIn: parent
                width: 190; height: 190; radius: 95
                color: "transparent"
                border.width: 8
                border.color: window.colorPrimary
                // Здесь можно добавить Canvas для кругового прогресса,
                // но даже сплошным кольцом это выглядит уже в 10 раз лучше
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 2

                Label {
                    text: Math.floor(remainingSeconds / 60) + ":" + (remainingSeconds % 60 < 10 ? "0" : "") + (remainingSeconds % 60)
                    font.pixelSize: 42
                    font.bold: true
                    color: window.colorTextMain
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: "ОСТАЛОСЬ"
                    font.pixelSize: 9
                    font.bold: true
                    color: window.colorTextLight
                    font.letterSpacing: 1.5
                    Layout.alignment: Qt.AlignHCenter
                    Layout.bottomMargin: 10
                }

                // СТИЛИЗОВАННАЯ КНОПКА УПРАВЛЕНИЯ
                Button {
                    id: timerControlBtn
                    text: isTimerRunning ? "ПАУЗА" : "СТАРТ"
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 36
                    Layout.alignment: Qt.AlignHCenter

                    background: Rectangle {
                        radius: 18
                        // Если таймер идет - кнопка светлая, если стоит - яркая
                        color: isTimerRunning ? "#F5F2EE" : window.colorPrimary
                        border.color: isTimerRunning ? "#DDD" : "transparent"

                        // Эффект нажатия
                        opacity: timerControlBtn.pressed ? 0.8 : 1.0
                    }

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 10
                        font.bold: true
                        color: isTimerRunning ? window.colorTextMain : "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: isTimerRunning = !isTimerRunning
                }
            }
        }

        // --- НИЖНЯЯ ПАНЕЛЬ КНОПОК ---
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Layout.margins: 20
            spacing: 15

            // КНОПКА НАЗАД (Стилизованная)
            Button {
                id: backBtn
                text: "←  НАЗАД"
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                visible: root.currentStep > 0 // Показываем только если это не первый шаг

                background: Rectangle {
                    radius: 15
                    color: backBtn.pressed ? "#F0EBE3" : "transparent"
                    border.color: "#DCDCDC"
                    border.width: 1
                }

                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 12
                    font.bold: true
                    color: window.colorTextLight // Нейтральный серый цвет текста
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    root.currentStep--;
                    root.updateStep();
                }
            }

            // КНОПКА ДАЛЕЕ / ЗАВЕРШИТЬ (Уже рыжая)
            Button {
                id: nextBtn
                text: root.currentStep === root.steps.length - 1 ? "ЗАВЕРШИТЬ 🎉" : "ДАЛЕЕ  →"
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                background: Rectangle {
                    radius: 15
                    color: nextBtn.pressed ? "#BF5A39" : window.colorPrimary
                    Behavior on color { ColorAnimation { duration: 100 } }
                }

                contentItem: Text {
                    text: parent.text
                    font.pixelSize: 12
                    font.bold: true
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (root.currentStep < root.steps.length - 1) {
                        root.currentStep++;
                        root.updateStep();
                    } else {
                        root.isTimerRunning = false;
                        root.visible = false;
                    }
                }
            }
        }
    }
}