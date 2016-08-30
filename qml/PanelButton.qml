import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Button {
    id:button
    property alias iconSource: image.source
    Layout.fillWidth: true
    contentItem: Image {
        id: image
        fillMode: Image.PreserveAspectFit
        sourceSize.width: 12
    }
    BrightnessContrast {
        anchors.fill: image
        source: image
        brightness: button.checkable ? button.checked ? 1 : 0 : 1
        contrast: 0.5
    }
}
