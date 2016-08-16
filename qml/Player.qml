import QtQuick 2.7

PlayerForm {
    displayTitle {
        text: mdxPlayer.title;
        property real scrollTo: displayTitle.contentWidth - displayTitle.width;
    }
    SequentialAnimation {
        running: true
        loops: Animation.Infinite;
        PropertyAnimation {
            properties: "leftPadding";
            target: displayTitle;
            from: 100;
            to: displayTitle.scrollTo;
            duration: 3000;
        }
        PauseAnimation { duration: 1000 }
        PropertyAnimation {
            properties: "leftPadding";
            target: displayTitle;
            from: displayTitle.scrollTo;
            to: 100;
            duration: 3000;
        }
        PauseAnimation { duration: 1000 }
    }

    displayFileName.text: mdxPlayer.fileName;
    buttonPlay {
        onClicked:{
            mdxPlayer.loadSong(true,"","",2,true);
            mdxPlayer.startPlay();
        }
    }

}
