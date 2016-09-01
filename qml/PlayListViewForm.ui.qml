import QtQuick 2.7
import QtQuick.Controls 1.4

Item {
    width: 400
    height: 400
    property alias playListView: playListView

    ScrollView{
        anchors.fill: parent
        ListView {
            id: playListView
            anchors.fill: parent
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 500
            highlight: Rectangle {
                color: "dimgray"
                border.color: "red"
            }
            model: playList

        }
    }
}
