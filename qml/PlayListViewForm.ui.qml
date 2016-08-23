import QtQuick 2.7
import QtQuick.Controls 2.0

Item {
    width: 400
    height: 400
    property alias playListView: playListView

    ListView {
        id: playListView
        anchors.fill: parent
        clip: true
        model: ListModel {
            id: playlist
        }
    }
}