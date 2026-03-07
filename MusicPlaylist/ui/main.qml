import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import "Helpers.js" as Logic

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
                text: "Play/Stop"
                onClicked: myAudioPlayer.togglePlay()
            }


            /**
             -------------------------
             might remove
             --------------------------
            Button{
                text: "Stop"
                onClicked: myAudioPlayer.stop()
            }
            
            **/
        
 

            FileDialog {
                id: filePicker
                title: "Select a Song"
                currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
                onAccepted: {
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
          
            border.color: "black"
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

        Button{
            text: "Clear Playlist"
            onClicked: myAudioPlayer.clearPlaylist()
        }

        Text {
            text: "Playing: " + myAudioPlayer.currentSongTitle
            font.pixelSize: 10
            color: "black"
        }
        
        Column{
            width: parent.width
            spacing: 5

            Slider {
                id: progressSlider
                width: parent.width
                from: 0
                to: myAudioPlayer.songDuration
                value: myAudioPlayer.songPosition

                onPressedChanged:{
                    if (!pressed){
                        myAudioPlayer.seek(value)
                    }
                }
            }
    
            RowLayout {
                width: parent.width

                Text { text: Logic.formatTime(myAudioPlayer.songPosition) }
                Item { Layout.fillWidth: true }
                Text { text: Logic.formatTime(myAudioPlayer.songDuration) }
            }
        }
    
        Row {
            spacing: 10
            Text { text: "volume: "; font.pixelSize: 15}

            Slider {
                id: volumeSlider
                from: 0.0
                to: 1.0
                value: myAudioPlayer.volume

                onMoved: {
                    myAudioPlayer.setVolume(value)
                }
            }
        }
    }   
}