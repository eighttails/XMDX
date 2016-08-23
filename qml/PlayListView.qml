import QtQuick 2.4

PlayListViewForm {
    playListView {
        delegate: PlayListViewDelegate {
            mdxTitle.text: title
            mdxFileName.text: fileName
            width: parent.width
            itemDelegate1.onClicked: {
                mdxPlayer.loadSong(true, fileName, "", 2, true);
                mdxPlayer.startPlay();
            }
        }
    }
}
