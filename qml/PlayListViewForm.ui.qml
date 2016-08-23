import QtQuick 2.7
import QtQuick.Controls 2.0

Item {
    width: 400
    height: 400
    property alias playListView: playListView

    ListView {
        id: playListView
        anchors.fill: parent
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 500
        highlight: Rectangle {
        color: "dimgray"
        border.color: "red"
        }

        clip: true
        model: ListModel {
            id: playlist
        }
    }
}
