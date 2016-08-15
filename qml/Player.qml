import QtQuick 2.4

PlayerForm {
    displayTitle.text: mdxPlayer.title;
    displayFileName.text: mdxPlayer.fileName;
    buttonPlay {
        onClicked:{
            mdxPlayer.loadSong(true,"","",2,true);
            mdxPlayer.startPlay();
        }
    }
}
