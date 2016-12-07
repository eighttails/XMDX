import QtQuick 2.7

PlayerForm {
    property alias isLoopEnabled: buttonLoop.checked
    property alias isRandomEnabled: buttonRandom.checked

    displayTitle {
        text: mdxPlayer.title

        // 曲名が画面に収まらない場合にスクロール
        SequentialAnimation on leftPadding {
            id: marqueeScroll
            property int scrollTo: Math.min(0, displayTitle.width - displayTitle.contentWidth)
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
        onTextChanged:{
            // 曲のタイトルが変わったらアニメーションを再定義する。(移動距離が変わるため)
            marqueeScroll.running = false;
            marqueeScroll.running = true;
        }
        onWidthChanged:{
            // 画面の向きが変わったらアニメーションを再定義する。(移動距離が変わるため)
            marqueeScroll.running = false;
            marqueeScroll.running = true;
        }

    }

    displayFileName.text: mdxPlayer.fileName;
    displayDuration.text: !mdxPlayer.isSongLoaded ?
                              "" :
                              "%1/%2"
                              .arg(mdxPlayer.currentPositionString)
                              .arg(mdxPlayer.durationString)

    buttonPlay {
        iconSource: mdxPlayer.isPlaying ? "qrc:/icon/openiconic/media-pause.svg" : "qrc:/icon/openiconic/media-play.svg"
        onClicked:{
            if (mdxPlayer.isPlaying){
                mdxPlayer.stopPlay();
            } else {
                mdxPlayer.startPlay();
            }
        }
    }
    buttonLoop {
        id:buttonLoop
    }
    buttonRandom {
        id:buttonRandom
    }

    sliderPlayPosition {
        value: mdxPlayer.duration == 0 ? 0 : mdxPlayer.currentPosition / mdxPlayer.duration
        onPressedChanged: {
            if(sliderPlayPosition.pressed){
                mdxPlayer.stopPlay();
            } else {
                mdxPlayer.setCurrentPosition(sliderPlayPosition.value * mdxPlayer.duration);
                mdxPlayer.startPlay();
            }
        }
    }

    // デバッグ用------------------------------------------
    rowLayoutDebugButtons{
        // デバッグに必要な場合のみtrueにする。
        visible: debug
    }

    buttonLoad  {
        onClicked:{
            playlistManager.loadPlaylist("test");
        }
    }
    buttonSave  {
        onClicked:{
            playlistManager.savePlaylist("test");
        }
    }
    buttonTrace  {
        onClicked:{
            console.log("currentPosition:", mdxPlayer.currentPosition);
            console.log("duration:", mdxPlayer.duration);
            console.log("playlist items:", playlist.count);
        }
    }
}
