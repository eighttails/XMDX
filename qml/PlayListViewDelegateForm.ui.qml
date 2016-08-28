import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Item {
    id: item1
    width: 400
    height: 30
    property alias mdxFileName: mdxFileName
    property alias mdxTitle: mdxTitle
    property alias itemDelegate1: itemDelegate1



    ItemDelegate {
        id: itemDelegate1
        anchors.fill: parent

        RowLayout {
            id: rowLayout1
            anchors.fill: parent

            Text {
                id: mdxTitle
                width: 50
                text: qsTr("TITLE")
                elide: Text.ElideRight
                color: "white"
                Layout.fillWidth: true
                anchors.verticalCenter: parent.verticalCenter
                style: Text.Raised
                anchors.left: parent.left
                anchors.leftMargin: 5
                font.pixelSize: 16
            }

            Item {
                id: spacing
                width: 10
            }

            Text {
                id: mdxFileName
                text: qsTr("FILENAME")
                clip: true
                elide: Text.ElideLeft
                color: "white"
                Layout.maximumWidth: 100
                Layout.fillWidth: false
                anchors.verticalCenter: parent.verticalCenter
                style: Text.Raised
                anchors.right: parent.right
                anchors.rightMargin: 5
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 12
            }

        }
    }
}
