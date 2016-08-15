import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    width: 320
    height: 200

    ColumnLayout {
        id: columnLayout1
        anchors.fill: parent

        Rectangle {
            id: rectangle1
            width: 200
            height: 100
            color: "#57a6b6"
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: false
            Layout.fillWidth: true

            Rectangle {
                id: rectangle2
                color: "#00000000"
                radius: 0
                border.width: 3
                border.color: "#3e5586"
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.bottomMargin: 5
                anchors.topMargin: 5
                anchors.fill: parent
                layer.enabled: true
            }

        }

        RowLayout {
            id: rowLayout1
            width: 100
            height: 100
            Layout.fillWidth: true

            Button {
                id: buttonRandom
                Layout.fillWidth: true
                iconSource: "qrc:/icon/random.svg"
            }
            Button {
                id: buttonBackward
                Layout.fillWidth: true
                iconSource: "qrc:/icon/media-step-backward.svg"
            }
            Button {
                id: buttonPlay
                Layout.fillWidth: true
                iconSource: "qrc:/icon/media-play.svg"
            }
            Button {
                id: buttonForward
                Layout.fillWidth: true
                iconSource: "qrc:/icon/media-step-forward.svg"
            }
            Button {
                id: buttonLoop
                Layout.fillWidth: true
                iconSource: "qrc:/icon/loop.svg"
            }
        }

        Slider {
            id: slider1
            Layout.fillHeight: false
            Layout.fillWidth: true
            value: 0.5
        }

        Item {
            width: 200
            height: 200
            Layout.fillHeight: true
            Layout.fillWidth: true
        }





    }



}
