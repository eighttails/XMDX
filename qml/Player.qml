import QtQuick 2.7

PlayerForm {
    displayTitle {
        text: mdxPlayer.title

        // 曲名が画面に収まらない場合にスクロール
        SequentialAnimation on leftPadding {
            id: marqueeScroll
            property int scrollTo: Math.min(0, displayTitle.width - displayTitle.contentWidth)
            running: mdxPlayer.isPlaying
            loops: Animation.Infinite;
            PauseAnimation { duration: 1000 }
            PropertyAnimation {
                from: 0
                to: marqueeScroll.scrollTo
                duration: Math.abs(marqueeScroll.scrollTo) * 20
            }
            PauseAnimation { duration: 1000 }
            PropertyAnimation {
                from: marqueeScroll.scrollTo
                to: 0
                duration: Math.abs(marqueeScroll.scrollTo) * 20
            }
        }
    }

    displayFileName.text: mdxPlayer.fileName;
    displayDuration.text: "%1/%2"
    .arg(mdxPlayer.currentPositionString)
    .arg(mdxPlayer.durationString)

    buttonPlay {
        iconSource: mdxPlayer.isPlaying ? "qrc:/icon/media-pause.svg" : "qrc:/icon/media-play.svg"
        onClicked:{
            if (mdxPlayer.isPlaying){
                mdxPlayer.stopPlay();
            } else {
                mdxPlayer.startPlay();
            }
        }
    }
    buttonLoop {
        onClicked:{
        }
    }

    sliderPlayPosition {
        value: mdxPlayer.duration == 0 ? 0 : mdxPlayer.currentPosition / mdxPlayer.duration
    }

    // デバッグ用------------------------------------------
    rowLayoutDebugButtons{
        // デバッグに必要な場合のみtrueにする。
        visible: debug
    }

    buttonLoad  {
        onClicked:{
            mdxPlayer.loadSong(true,"","",2,true);
        }
    }
    buttonTrace  {
        onClicked:{
            console.log("currentPosition:", mdxPlayer.currentPosition);
            console.log("duration:", mdxPlayer.duration);
        }
    }
}
