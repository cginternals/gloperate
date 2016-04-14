
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0
import gloperate.ui 1.0


Dialog {
    id: video
    title: qsTr("Videocapture")
    standardButtons: StandardButton.Ok | StandardButton.Cancel

    RowLayout {
        ColumnLayout {
            Row { Label {text: qsTr("Filename:")} }
            Row { Label {text: qsTr("FPS:")} }
            Row { Label {text: qsTr("Seconds:")} }
            Row { Label {text: qsTr("Width:")} }
            Row { Label {text: qsTr("Height:")} }
        }

        ColumnLayout {
            Row {
                id: rowFilename
                TextField {
                    id:filename
                    text: "output-video.avi"
                }
                // Label {text: ".avi"}
            }
            Row {
                id: rowFps
                TextField {
                    id:fps
                    text: "30"
                }
            }
            Row {
                id: rowSeconds
                TextField {
                    id:seconds
                    text: "5"
                }
            }
            Row {
                id: rowWidth
                TextField {
                    id:width
                    text: "1280"
                }
            }
            Row {
                id: rowHeight
                TextField {
                    id:height
                    text: "720"
                }
            }
        }
    }

    onButtonClicked: {
        if (clickedButton==StandardButton.Ok) {
            close();
            gloperate.surface0.createVideo(filename.text, fps.text, seconds.text, width.text, height.text);
        }
    }

    // Component.onCompleted: open()
}

// DockPanel
// {
//     id: video

//     iconClosed: '0270-cancel-circle.png'
//     iconOpen:   '0270-cancel-circle.png'

//     property string video_filename: "output-video.avi"
//     property int video_fps: 30
//     property int video_length: 5
//     property int video_width: 1280
//     property int video_height: 720

//     RowLayout {
//         id: layout
//         anchors.fill: parent

//         Item {
//             width: 10
//             height: 1
//         }

//         GridLayout {
//             columns: 2
//             rowSpacing: 10

//             Layout.fillHeight: true
//             Layout.fillWidth: true

//             Text {
//                 text: "Hello world? "
//             }
//             Text {
//                 text: "Hello world!"
//             }

//             Text {
//                 text: "Goodbye world? "
//             }
//             Text {
//                 text: "Goodbye world!"
//             }

//         }

//         Item {
//             width: 10
//             height: 10
//         }
//     }

//     Button {
//         text: "Create Video"
//         onClicked: {
//             video.hidePanel();
//             gloperate.surface.createVideo(video_filename, video_fps, video_length, video_width, video_height);
//         }
//     }
// }
