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
                        id: playlistDelegate
                        width: playlistManager.width
                    
                        contentItem: Text{
                            text: modelData
                            color: "black"
                            leftPadding: 10
                        }
                    
                        onClicked: myAudioPlayer.loadPlaylist(modelData)

                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton
                            onClicked: (mouse) => {
                                if(mouse.button === Qt.RightButton) {
                                    contextMenu.playlistNameToDelete = modelData
                                    contextMenu.popup()
                                }
                            }
                        }
                    }
                }
            }

            Menu {
                id: contextMenu
                property string playlistNameToDelete: ""

                MenuItem {
                    text: "Delete Playlist"
                    onTriggered: {
                        myAudioPlayer.playlistManager.deletePlaylist(contextMenu.playlistNameToDelete)
                    }
                }
            }
    
            Button{
                text: "Add Playlist"
                onClicked: addPlaylistDialog.open()
            }
            
            Dialog{
                id: addPlaylistDialog
                title: "Create New Playlist"
                standardButtons: Dialog.Ok  | Dialog.Cancel
                anchors.centerIn: parent

                property bool nameExists: myAudioPlayer.playlistManager.checkPlaylist(newNameField.text)
                
                Column {
                    spacing: 10
                    Text { text: "Playlist Name:"}
                    TextField{
                        id: newNameField
                        placeholderText: "Enter name..."    
                        width: 150

                        background: Rectangle{
                            border.color: addPlaylistDialog.nameExists ? "red" : "gray"
                            border.width: 1
                        }
                    }
                    
                    Text {
                        text: "That playlist name already exists!"
                        color: "red"
                        visible: addPlaylistDialog.nameExists   
                        font.pixelSize: 11
                    }
                }
                
                Component.onCompleted: {
                    standardButton(Dialog.Ok).enabled = Qt.binding(function() { 
                        return newNameField.text.length > 0 && !addPlaylistDialog.nameExists 
                    })
                }
            
                onAccepted: {
                    myAudioPlayer.playlistManager.createPlaylist(newNameField.text)
                    newNameField.text = ""
                }
            }
        
        }
        Column{
            width: parent.width * 0.7
            spacing: 10
        
            Text{
                text: myAudioPlayer.currentPlaylistName
                font.pixelSize: 15
            }

            Rectangle{
                width: parent.width ; height: 200
          
                border.color: "black"
                clip: true
                ListView{
                    id: playlistView
                    anchors.fill: parent
                    model: myAudioPlayer.currentPlaylistSongs     
   


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
