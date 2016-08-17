import QtQuick 2.7

PlayerForm {
    displayTitle {
        text: mdxPlayer.title
        SequentialAnimation on leftPadding {
            id: marqueeScroll
            property int scrollTo: Math.min(0, displayTitle.width - displayTitle.contentWidth)
            running: mdxPlayer.isPlaying
            loops: Animation.Infinite;
            PauseAnimation { duration: 1000 }
            PropertyAnimation {
                from: 0
                to: marqueeScroll.scrollTo
                duration: Math.abs(marqueeScroll.scrollTo) * 10
            }
            PauseAnimation { duration: 1000 }
            PropertyAnimation {
                from: marqueeScroll.scrollTo
                to: 0
                duration: Math.abs(marqueeScroll.scrollTo) * 10
            }
        }
    }


    displayFileName.text: mdxPlayer.fileName;
    buttonPlay {
        onClicked:{
            mdxPlayer.loadSong(true,"","",2,true);
            mdxPlayer.startPlay();
        }
    }
    buttonLoop  {
        onClicked:{
            console.log("contentwidth:",displayTitle.contentWidth);
            console.log("width:",displayTitle.width);
            console.log("scrollTo:",displayTitle.scrollTo);

        }
    }

}
