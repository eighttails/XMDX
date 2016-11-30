import QtQuick 2.4

PlaylistViewDelegateForm {
    mdxTitle.text: title
    mdxFileName.text: playlistManager.fileNameFromPath(fileName)
    width: parent.width

    property bool isCurrentItem: ListView.isCurrentItem

    itemDelegate1 {
        onClicked: {
            mdxPlayer.playFileByIndex(index);
        }
    }

    onIsCurrentItemChanged: {
        if(isCurrentItem){
            mdxPlayer.loadSong(true, fileName, "", 2, true);
            mdxPlayer.startPlay();
        }
    }
}
