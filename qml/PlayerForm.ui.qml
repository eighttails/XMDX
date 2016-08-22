import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 320
    height: columnLayout1.height

    property alias buttonTrace: buttonTrace
    property alias rowLayoutDebugButtons: rowLayoutDebugButtons
    property alias buttonLoad: buttonLoad
    property alias displayDuration: displayDuration
    property alias displayFileName: displayFileName
    property alias displayTitle: displayTitle
    property alias sliderPlayPosition: sliderPlayPosition
    property alias buttonLoop: buttonLoop
    property alias buttonForward: buttonForward
    property alias buttonPlay: buttonPlay
    property alias buttonBackward: buttonBackward
    property alias buttonRandom: buttonRandom
    property alias buttonMenu: buttonMenu

    ColumnLayout {
        id: columnLayout1
        width:parent.width

        Rectangle {
            id: displayPanel
            width: 200
            height: 100
            color: "steelblue"
            Layout.fillWidth: true

            GridLayout {
                id: gridLayout1
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                anchors.bottomMargin: 10
                anchors.topMargin: 10
                columns: 2
                rows: 1
                anchors.fill: parent

                Text {
                    id: displayTitle
                    color: "skyblue"
                    text: qsTr("曲名")
                    clip: true
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    font.pixelSize: 24
                    style: Text.Raised
                    textFormat: Text.PlainText
                }

                Text {
                    id: displayFileName
                    color: "skyblue"
                    text: qsTr("ファイル名")
                    clip: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                    font.pixelSize: 18
                    style: Text.Raised
                }
                Text {
                    id: displayDuration
                    color: "skyblue"
                    text: qsTr("00:00")
                    clip: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                    font.family: "Arial"
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 18
                    style: Text.Raised
                }
            }

            Rectangle {
                id: rectangle2
                color: "#00000000"
                radius: 0
                z: 3
                border.width: 3
                border.color: "skyblue"
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.bottomMargin: 5
                anchors.topMargin: 5
                anchors.fill: parent
                layer.enabled: true
            }

            Rectangle {
                id: rectangle3
                color: "#00000000"
                radius: 0
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.bottomMargin: 4
                anchors.topMargin: 6
                anchors.fill: parent
                z: 2
                border.width: 3
                layer.enabled: true
                border.color: "#000000"
            }
        }

        RowLayout {
            id: rowLayoutButtons
            Layout.fillWidth: true

            Button {
                id: buttonMenu
                Layout.fillWidth: true
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icon/menu.svg"

                }
            }

            Button {
                id: buttonRandom
                checkable: true
                Layout.fillWidth: true
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icon/random.svg"
                }
            }
            Button {
                id: buttonBackward
                Layout.fillWidth: true
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icon/media-step-backward.svg"
                }
            }
            Button {
                id: buttonPlay
                Layout.fillWidth: true
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icon/media-play.svg"
                }
            }
            Button {
                id: buttonForward
                Layout.fillWidth: true
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icon/media-step-forward.svg"
                }
            }
            Button {
                id: buttonLoop
                checkable: true
                Layout.fillWidth: true
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/icon/loop.svg"
                }
            }
        }

        RowLayout {
            id: rowLayoutDebugButtons

            Button {
                id: buttonLoad
                text: qsTr("LOAD")
            }

            Button {
                id: buttonTrace
                text: qsTr("TRACE")

            }
        }

        Slider {
            id: sliderPlayPosition
            Layout.fillWidth: true
        }
    }
}
