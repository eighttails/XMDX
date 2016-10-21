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
            //TODO シャッフル再生
        } else {
            if(settings.loop &&  playListView.listViewBody.currentIndex == playListView.listViewBody.count - 1){
                // リピート再生時はリストの最後まで行ったら先頭に戻る
                playListView.listViewBody.currentIndex = 0;
            } else {
                playListView.listViewBody.incrementCurrentIndex();
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
            //TODO シャッフル再生
        } else {
            if(settings.loop &&  playListView.listViewBody.currentIndex == 0){
                // リピート再生時はリストの先頭まで行ったら末尾に戻る
                playListView.listViewBody.currentIndex = playListView.listViewBody.count - 1;
            } else {
                playListView.listViewBody.decrementCurrentIndex();
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

        PlayListView{
            id: playListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            listViewBody.model: playList
        }

        Menu{
            id: mainMenu
            font.pixelSize: 14
            MenuItem{
                id: menuNewPlayList
                text: "新規プレイリスト"
                onTriggered: {
                    appHelper.clearPlayList();
                }
            }
            MenuItem{
                id: menuAddFile
                text: "ファイルを追加..."
                onTriggered: {
                    var mdxFile = appHelper.addFileDialog();
                    if (mdxFile.localeCompare("") !== 0){
                        appHelper.addFile(mdxFile);
                    }
                }
            }
            MenuItem{
                id: menuAddFolder
                text: "フォルダを追加..."
                onTriggered: {
                    var folder = appHelper.addFolderDialog();
                    if (folder.localeCompare("") !== 0){
                        appHelper.addFolder(folder);
                    }
                }
            }
            MenuItem{
                id: menuLoadPlayList
                text: "プレイリストを読み込み..."
            }
            MenuItem{
                id: menuSavePlayList
                text: "プレイリストを保存..."
            }
            MenuItem{
                id: menuSettings
                text: "設定..."
            }
        }
    }
}
