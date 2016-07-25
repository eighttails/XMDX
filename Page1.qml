import QtQuick 2.7

Page1Form {
    property bool songLoaded : false;
    button1.onClicked: {
        console.log("Button 1 clicked.");
        if(!songLoaded){
            mdxPlayer.loadSong(true,
                               "",
                               "",
                               2, true);
            songLoaded = true;
        }
        mdxPlayer.startPlay();
        button1.text = mdxPlayer.title;
    }
    button2.onClicked: {
        console.log("Button 2 clicked.");
        mdxPlayer.stopPlay();
    }
}
