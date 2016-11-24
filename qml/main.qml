import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtMultimedia 5.6
import QtQuick.Controls.Material 2.0

import Qt.labs.settings 1.0

ApplicationWindow {
    visible: true
    width: 320
    height: 480
    Material.theme: Material.Dark
    Material.accent: Material.Blue

    Settings{
        id:settings
        property alias random: playerPanel.isRandomEnabled
        property alias loop: playerPanel.isLoopEnabled
    }

    // 次の曲に移動
    function stepForward(){
        if(settings.random){
            // ランダム再生
            var index = playlistManager.nextRandom(settings.loop);
            if( index >= 0){
                playlistView.listViewBody.currentIndex = index;
            }
        } else {
            if(settings.loop &&  playlistView.listViewBody.currentIndex == playlistView.listViewBody.count - 1){
                // ループ再生時はリストの最後まで行ったら先頭に戻る
                playlistView.listViewBody.currentIndex = 0;
            } else {
                playlistView.listViewBody.incrementCurrentIndex();
            }
        }
    }

    // 前の曲に戻る
    function stepBackward(){
        // 曲開始から5秒以上経っていたら、前の曲には戻らずに現在の曲を頭出し
        if(mdxPlayer.currentPosition > 5) {
            mdxPlayer.currentPosition = 0;
            return;
        }

        if(settings.random){
            // ランダム再生
            var index = playlistManager.previousRandom(settings.loop);
            if( index >= 0){
                playlistView.listViewBody.currentIndex = index;
            }
        } else {
            if(settings.loop &&  playlistView.listViewBody.currentIndex == 0){
                // ループ再生時はリストの先頭まで行ったら末尾に戻る
                playlistView.listViewBody.currentIndex = playlistView.listViewBody.count - 1;
            } else {
                playlistView.listViewBody.decrementCurrentIndex();
            }
        }
    }

    Connections {
        target: mdxPlayer
        onSongPlayFinished: {
            stepForward();
        }
    }

    GridLayout {
        id: flow1
        anchors.fill: parent
        flow: width > height ? GridLayout.LeftToRight : GridLayout.TopToBottom
        Player{
            id: playerPanel
            Layout.fillWidth: true
            buttonMenu {
                onClicked: {
                    mainMenu.open();
                }
            }
            buttonBackward {
                onClicked: {
                    stepBackward();
                }
            }
            buttonForward {
                onClicked: {
                    stepForward();
                }
            }
        }

        PlaylistView{
            id: playlistView
            Layout.fillWidth: true
            Layout.fillHeight: true
            listViewBody.model: playlist
        }

        Menu{
            id: mainMenu
            font.pixelSize: 14
            MenuItem{
                id: menuNewPlaylist
                text: "新規プレイリスト"
                onTriggered: {
                    playlistManager.clearPlaylist();
                }
            }
            MenuItem{
                id: menuAddFile
                text: "ファイルを追加..."
                onTriggered: {
                    var mdxFile = playlistManager.addFileDialog();
                    if (mdxFile.localeCompare("") !== 0){
                        playlistManager.addFile(mdxFile);
                    }
                }
            }
            MenuItem{
                id: menuAddFolder
                text: "フォルダを追加..."
                onTriggered: {
                    var folder = playlistManager.addFolderDialog();
                    if (folder.localeCompare("") !== 0){
                        playlistManager.addFolder(folder);
                    }
                }
            }
            MenuItem{
                id: menuLoadPlaylist
                text: "プレイリストを読み込み..."
            }
            MenuItem{
                id: menuSavePlaylist
                text: "プレイリストを保存..."
            }
            MenuItem{
                id: menuSettings
                text: "設定..."
            }
        }
    }
}
