import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtMultimedia 5.6
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    visible: true
    width: 320
    height: 480
    Material.theme: Material.Dark
    Material.accent: Material.Blue

    Connections {
        target: mdxPlayer
        onSongPlayFinished: {
            var shuffle = false; // TODO 設定から取得
            var repeat = true; // TODO 設定から取得
            if(shuffle){
                //TODO シャッフル再生
            } else {
                if(repeat &&  playListView.listViewBody.currentIndex == playListView.listViewBody.count - 1){
                    // リピート再生時はリストの最後まで行ったら先頭に戻る
                    playListView.listViewBody.currentIndex = 0;
                } else {
                    playListView.listViewBody.incrementCurrentIndex();
                }
            }
        }
    }

    GridLayout {
        id: flow1
        anchors.fill: parent
        flow: width > height ? GridLayout.LeftToRight : GridLayout.TopToBottom

        Player{
            Layout.fillWidth: true
            buttonMenu {
                onClicked:{
                    mainMenu.open();
                }
            }
        }

        PlayListView{
            id: playListView
            visible:true
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
