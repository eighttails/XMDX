import QtQuick 2.7

Page1Form {
    button1.onClicked: {
        console.log("Button 1 clicked.");
        mdxPlayer.loadSong(true,
                           "",
                           "",
                           2, true);
        mdxPlayer.startPlay();
    }
    button2.onClicked: {
        console.log("Button 2 clicked.");
        mdxPlayer.stopPlay();
    }
}
