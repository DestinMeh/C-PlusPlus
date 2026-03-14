import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import "Helpers.js" as Logic



ApplicationWindow {
    id:window
    visible: true
    width: 640
    height: 400
    title: "Music Ahh player"

    // --- COLOR PALETTE ---
    readonly property color colorBg: "#121212"
    readonly property color colorSidebar: "#1e1e1e"
    readonly property color colorCard: "#282828"
    readonly property color colorAccent: "#1DB954" // Spotify Green
    readonly property color colorText: "#FFFFFF"
    readonly property color colorTextDim: "#B3B3B3"

    background: Rectangle { color: colorBg }

    RowLayout{
        anchors.fill: parent
        spacing: 0

        // ---- SideBar ---

        Rectangle{
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.25
            color: colorSidebar

            ColumnLayout{
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Text{
                    text: "Library"
                    color: colorText
                    font.pixelSize: 20
                    // font.bold:True
                }

                ListView {
                    id: playlistManager
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: myAudioPlayer.playlistManager.playlistNames
                    spacing: 4
                    clip: true

                    delegate: ItemDelegate{
                        width: playlistManager.width
                        height: 40

                        background: Rectangle{
                            radius: 6
                            color: myAudioPlayer.currentPlaylistName === modelData ?
                                       Qt.rgba(1, 1, 1, 0.1) : "transparent"
                        }

                        contentItem: Text{
                            text: modelData
                            color: myAudioPlayer.currentPlaylistName === modelData ?
                                       colorAccent : colorTextDim
                            font.pixelSize: 14
                            verticalAlignment: Text.AlignVCenter
                            leftPadding:10
                        }

                        onClicked: myAudioPlayer.loadPlaylist(modelData)

                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton
                            onClicked: (mouse) => {
                                           if(mouse.button === Qt.RightButton) {
                                               contextMenu.playlistNameToDelete = modelData
                                               contetMenu.popup()
                                           }
                                       }
                        }
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: "+ New Playlist"
                    onClicked: addPlaylistDialog.open()

                    background: Rectangle{
                        radius:10
                        color: parent.down ? "#1AA34A" : colorAccent
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                    }
                }
            }
        }

        // -- Main Content Area --
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 25
            spacing: 20

            // Playlist Header
            Text {
                text: myAudioPlayer.currentPlaylistName || "No Playlist Selected"
                color: colorText
                font.pixelSize: 32
                font.bold: true
            }

            // Song List Card
            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: colorCard
                radius: 5
                clip: true

                ListView{
                    id: playlistView
                    anchors.fill: parent
                    anchors.margins: 5
                    model: myAudioPlayer.currentPlaylistSongs
                    delegate: ItemDelegate{
                        width: playlistView.width
                        contentItem: RowLayout {
                            Text{
                                text: modelData.title
                                color: colorText
                                Layout.fillWidth: true
                                leftPadding: 10
                            }
                        }
                        onClicked: myAudioPlayer.playFromPlaylist(index)
                    }
                }
            }

            // --- player controls section --
            ColumnLayout{
                Layout.fillWidth: true
                spacing : 10

                Text{
                    text: myAudioPlayer.currentSongTitle ? "Now Playing: " + myAudioPlayer.currentSongTitle : "Select a song"
                    color: colorAccent
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignHCenter

                }

                // progress bar
                Slider{
                    id: progressSlider
                    Layout.fillWidth: true
                    from: 0
                    to: myAudioPlayer.songDuration
                    value: myAudioPlayer.songPosition

                    background: Rectangle{
                        height: 4
                        width: progressSlider.availableWidth
                        y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                        radius: 2
                        color: "#404040"
                        Rectangle{
                            width: progressSlider.visualPosition * parent.width
                            height: parent.height
                            color: colorAccent
                            radius: 2
                        }

                    }

                    onPressedChanged: if (!pressed) myAudioPlayer.seek(value)
                }

                RowLayout {
                    Layout.fillWidth: true
                    Text { text: Logic.formatTime(myAudioPlayer.songPosition); color: colorTextDim; font.pixelSize: 12 }
                    Item { Layout.fillWidth: true }
                    Text { text: Logic.formatTime(myAudioPlayer.songDuration); color: colorTextDim; font.pixelSize: 12 }
                }

                // Control Buttons
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 20

                    Button {
                        text: "Shuffle"
                        flat: true
                        onClicked: myAudioPlayer.shuffle()
                    }

                    RoundButton {
                        width: 60; height: 60
                        text: myAudioPlayer.isPlaying ? "Pause" : "Play"
                        palette.button: colorText
                        palette.buttonText: "black"
                        font.bold: true
                        onClicked: myAudioPlayer.togglePlay()
                    }

                    Button {
                        text: "Add Song"
                        onClicked: filePicker.open()
                    }

                    Button {
                        text: "Clear"
                        onClicked: myAudioPlayer.clearPlaylist()
                    }
                }

                // Volume Slider
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 10
                    Text { text: "Vol"; color: colorTextDim }
                    Slider {
                        id: volumeSlider
                        from: 0.0; to: 1.0; value: myAudioPlayer.volume
                        onMoved: myAudioPlayer.setVolume(value)
                    }
                }
            }
        }


    } 
}
