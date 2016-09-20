import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Item {
    id: item1
    width: 400
    height: 40

    property alias mdxFileName: mdxFileName
    property alias mdxTitle: mdxTitle
    property alias itemDelegate1: itemDelegate1

    ItemDelegate {
        id: itemDelegate1
        anchors.fill: parent

        ColumnLayout {
            id: columnLayout1
            anchors.fill: parent

            Text {
                id: mdxTitle
                text: qsTr("TITLE")
                clip: false
                elide: Text.ElideRight
                color: "white"
                Layout.fillWidth: true
                Layout.topMargin: 2
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                style: Text.Raised
                font.pixelSize: 16
            }

            Text {
                id: mdxFileName
                text: qsTr("FILENAME")
                horizontalAlignment: Text.AlignRight
                clip: false
                elide: Text.ElideLeft
                color: "white"
                Layout.fillWidth: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.bottomMargin: 2
                style: Text.Raised
                font.pixelSize: 12
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#00000000"
            border.color: "gray"
        }
    }
}
