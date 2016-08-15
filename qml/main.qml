import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    visible: true
    width: 320
    height: 480
    Material.theme: Material.Dark
    Material.accent: Material.Red

    Flow {
        id: flow1
        anchors.fill: parent
    }

    Player{
        anchors.fill: parent
    }

}
