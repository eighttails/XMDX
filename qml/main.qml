import QtQuick 2.0
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
            visible:true
            Layout.fillWidth: true
            Layout.fillHeight: true
            playListView {
                model: Playlist {
                    id: playlist
                    PlaylistItem { source: "song1.ogg"; }
                    PlaylistItem { source: "song2.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                    PlaylistItem { source: "song3.ogg"; }
                }
            }
        }

        Menu{
            id: mainMenu
            font.pixelSize: 14
            MenuItem{
                id: menuNewPlayList
                text: "新規プレイリスト"
            }
            MenuItem{
                id: menuAddFile
                text: "ファイルを追加..."
            }
            MenuItem{
                id: menuAddFolder
                text: "フォルダを追加..."
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
