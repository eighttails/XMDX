import QtQuick 2.4

PlaylistViewDelegateForm {
    mdxTitle.text: title
    mdxFileName.text: playlistManager.fileNameFromPath(fileName)
    width: parent.width

    property bool isCurrentItem: ListView.isCurrentItem

    itemDelegate1 {
        onClicked: {
            mdxPlayer.playFileByIndex(index);
            mdxPlayer.startPlay();
        }
    }
}
