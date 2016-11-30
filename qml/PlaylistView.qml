import QtQuick 2.4

PlaylistViewForm {
    property alias currentIndex : listViewBody.currentIndex
    listViewBody {
        id:listViewBody
        delegate: PlaylistViewDelegate{
        }
    }
}
