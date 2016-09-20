import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4

Item {
    width: 400
    height: 400
    property alias listViewBody: listViewBody
    clip: true

    ListView {
        id: listViewBody
        anchors.fill: parent
        ScrollBar.vertical: ScrollBar{}
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 500
        highlight: Rectangle {
            color: "dimgray"
            border.color: "red"
            border.width: 3
        }
    }
}
