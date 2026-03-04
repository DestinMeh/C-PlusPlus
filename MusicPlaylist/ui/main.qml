import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Music Player"

    Column{
        anchors.centerIn:parent
        spacing: 10

        Button {
            text: "Play"
            onClicked: myAudioPlayer.play("path")
        }

        Button{
            text: "Stop"
            onClicked: myAudioPlayer.stop()
        }
        
        Component{
            id: songDelegate
            Row {
                Text{ text: model.title}
                Button{ text: "Play"; onClicked: myAudioPlayer.play(model.filePath)}
            }
        }


        FileDialog {
            id: filePicker
            title: "Select a Song"
            currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
            onAccepted: {
                // Challenge: Convert selectedFile to a local path and 
                // call your C++ addToPlaylist() function here.
                myAudioPlayer.addToPlaylist(filePicker.selectedFile)
            }
        }

        Button {
            text: "Add Song to Playlist"
            onClicked: filePicker.open()
        }

        Text {
            text: "Songs in Playlist: " + myAudioPlayer.playlistCount
            font.pixelSize: 20
            color: "white"
        }

        Row {
            spacing: 5
            ListView{
                width: parent.width
                height: 200
                model: myAudioPlay.playlistCount     

                delegate: ItemDelegate{
                    width: parent.width
                    text: "Song #" + index
                    onClicked: myAudioPlayer.playFromPlaylist(index)
                }
            }
        }
    }
}