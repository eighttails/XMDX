import QtQuick 2.4

PlayListViewDelegateForm {
    mdxTitle.text: title
    mdxFileName.text: appHelper.fileNameFromPath(fileName)
    width: parent.width

    property bool isCurrentItem: ListView.isCurrentItem

    itemDelegate1 {
        onClicked: {
            ListView.currentIndex = index;
        }
    }

    onIsCurrentItemChanged: {
        if(isCurrentItem){
            mdxPlayer.loadSong(true, fileName, "", 2, true);
            mdxPlayer.startPlay();
        }
    }
}
