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
    Row{
        anchors.fill: parent

        Column{
            width: parent.width * 0.3
            Rectangle{
                width: parent.width ; 
                height: 200
          
                border.color: "black"
                clip: true
                ListView{
                    id: playlistManager
                    anchors.fill: parent
                    model: myAudioPlayer.playlistManager.playlistNames     
   

                    delegate: ItemDelegate{
                        width: playlistManager.width
                    
                        contentItem: Text{
                            text: modelData
                            color: "black"
                            leftPadding: 10
                        }
                    
                        onClicked: myAudioPlayer.loadPlaylist(modelData)
                    }
                }
            }
    
            Button{
                text: "Add Playlist"
                onClicked: myAudioPlayer.playlistManager.createPlaylist("New Playlist")
            }
        }
        Column{
            width: parent.width * 0.7
            spacing: 10
        
            

            Rectangle{
                width: parent.width ; height: 200
          
                border.color: "black"
                clip: true
                ListView{
                    id: playlistView
                    anchors.fill: parent
                    model: myAudioPlayer.currentPlaylistObjects     
   


                    delegate: ItemDelegate{
                        width: playlistView.width
                    
                        contentItem: Text{
                            text: modelData.title
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
        
            Row{
                spacing: 5
                
                Button {
                    text: "Play/Stop"
                    onClicked: myAudioPlayer.togglePlay()
                }
        
 

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
}