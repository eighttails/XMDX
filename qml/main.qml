import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    visible: true
    width: 320
    height: 480
    Material.theme: Material.Dark
    Material.accent: Material.Blue

    Flow {
        id: flow1
        anchors.fill: parent

        Player{
            buttonMenu {
                onClicked:{
                    mainMenu.open();
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
