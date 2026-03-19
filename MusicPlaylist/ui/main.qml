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
    readonly property color colorBg: "#d1cdb7"
    readonly property color colorSidebar: "#bab5a1"
    readonly property color colorCard: "#bab5a1"
    readonly property color colorAccent: "#454138" // Spotify Green
    readonly property color colorText: "#454138"
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
                                       Qt.rgba(1, 1, 1, 0.1) :
                                        (parent.hovered ? Qt.rgba(1,1,1,0.05) : "transparent")
                                   
                            Behavior on color { ColorAnimation { duration: 150 }} 
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
                                               contextMenu.popup()
                                           }
                                       }
                        }
                    }
                }

                Button {
                    id: addNewBtn
                    Layout.fillWidth: true
                    text: "+ New Playlist"
                    hoverEnabled: true // Required for the transition to trigger
                    onClicked: addPlaylistDialog.open()

                    // This handles the transition timing and logic
                    background: Rectangle {
                        implicitHeight: 40
                        radius: 10
                        color: "transparent" // The base button is transparent
                        border.color: colorAccent
                        border.width: 1
                        clip: true // Keeps the "after" transition inside the rounded corners

                        // The "::after" equivalent: The sliding background layer
                        Rectangle {
                            id: slideLayer
                            height: parent.height
                            width: addNewBtn.hovered ? parent.width : 0 // Slide from 0 to full width
                            color: colorAccent // Your #454138 color

                            // The CSS 'ease-out 0.2s' equivalent
                            Behavior on width {
                                NumberAnimation {
                                    duration: 200
                                    easing.type: Easing.OutQuad
                                }
                            }
                        }
                    }

                    contentItem: Text {
                        text: addNewBtn.text
                        // Switch text color so it's readable when the dark background slides in
                        color: addNewBtn.hovered ? "#d1cdb7" : colorAccent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true

                        // Smoothly transition the text color too
                        Behavior on color { ColorAnimation { duration: 200 } }
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
                radius: 0
                border.color: Qt.rgba(1,1,1,0.05)
                border.width: 1
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
                    text: myAudioPlayer.currentSongTitle ? myAudioPlayer.currentSongTitle : "Select a song"
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

                    background: Rectangle {
                            x: progressSlider.leftPadding
                            y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 4
                            width: progressSlider.availableWidth
                            height: implicitHeight
                            radius: 2
                            color: "#bab5a1" // Darker tan for the empty track

                            // The "Progress" part of the bar (filled area)
                            Rectangle {
                                width: progressSlider.visualPosition * parent.width
                                height: parent.height
                                color: colorAccent // Your #454138 color
                                radius: 2
                            }
                        }

                    handle: Rectangle {
                            x: progressSlider.leftPadding + progressSlider.visualPosition * (progressSlider.availableWidth - width)
                            y: progressSlider.topPadding + progressSlider.availableHeight / 2 - height / 2
                            implicitWidth: 14
                            implicitHeight: 14
                            radius: 7 // Makes it a circle
                            color: progressSlider.pressed ? colorAccent : "#f0ede1" // Highlights when pressed
                            border.color: colorAccent
                            border.width: 1

                            // Smoothly scale the handle when hovering
                            scale: progressSlider.hovered ? 1.2 : 1.0
                            Behavior on scale { NumberAnimation { duration: 100 } }
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

                        onClicked: myAudioPlayer.shuffle()
                    }

                    RoundButton {
                        width: 50; height: 50
                        
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

    // Popups and Dialogs

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

        // The Creation Dialog
        Dialog {
            id: addPlaylistDialog
            title: "Create New Playlist"
            standardButtons: Dialog.Ok | Dialog.Cancel
            anchors.centerIn: parent
            modal: true // Dims the background for a modern look

            // Logic to check if name is valid
            property bool nameExists: myAudioPlayer.playlistManager.checkPlaylist(newNameField.text)

            contentItem: ColumnLayout {
                spacing: 15
                Text {
                    text: "Playlist Name:"
                    color: "black" // Dialogs often use system themes; force black for visibility
                }
                TextField {
                    id: newNameField
                    placeholderText: "Enter name..."
                    Layout.fillWidth: true
                    selectByMouse: true
                }
                Text {
                    text: "That name already exists!"
                    color: "red"
                    visible: addPlaylistDialog.nameExists
                    font.pixelSize: 11
                }
            }

            // Enable OK button only if text is entered and doesn't exist
            onOpened: {
                standardButton(Dialog.Ok).enabled = Qt.binding(function() {
                    return newNameField.text.length > 0 && !addPlaylistDialog.nameExists
                })
            }

            onAccepted: {
                myAudioPlayer.playlistManager.createPlaylist(newNameField.text)
                newNameField.text = "" // Reset for next time
            }
        }

        // The File Picker (Already in your code, but ensure it's here)
        FileDialog {
            id: filePicker
            title: "Select a Song"
            onAccepted: {
                myAudioPlayer.addToPlaylist(filePicker.selectedFile)
            }
        }
}
