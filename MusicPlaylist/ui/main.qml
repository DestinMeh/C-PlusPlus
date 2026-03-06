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

        Row{
            spacing: 5

            Button {
                text: "Play"
                onClicked: myAudioPlayer.play("path")
            }

            Button{
                text: "Stop"
                onClicked: myAudioPlayer.stop()
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
                text: "Shuffle"
                onClicked: myAudioPlayer.shuffle()
            }

            Button {
                text: "Add Song to Playlist"
                onClicked: filePicker.open()
            }
        }

        Rectangle{
            width: parent.width ; height: 200
            color: "#222"
            border.color: "white"
            clip: true
            ListView{
                id: playlistView
                anchors.fill: parent
                model: myAudioPlayer.playlist     
   


                delegate: ItemDelegate{
                    width: playlistView.width
                    
                    contentItem: Text{
                        text: modelData.split('/').pop()
                        color: "black"
                        leftPadding: 10
                    }
                    
                    onClicked: myAudioPlayer.playFromPlaylist(index)
                }
            }
        }

        Text {
            text: "Playing: " + myAudioPlayer.currentSongTitle
            font.pixelSize: 10
            color: "black"
        }

        Button{
            text: "Clear Playlist"
            onClicked: myAudioPlayer.clearPlaylist()
        }
    }
}