
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0
import gloperate.ui 1.0


DockPanel
{
    id: video

    iconClosed: '0270-cancel-circle.png'
    iconOpen:   '0270-cancel-circle.png'

    signal createVideo()

    property string video_filename: "output_video.mp4"
    property int video_fps: 30
    property int video_length: 10
    property int video_width: 1600
    property int video_height: 900

    RowLayout {
        id: layout
        anchors.fill: parent

        Item {
            width: 10
            height: 1
        }

        GridLayout {
            columns: 2
            rowSpacing: 10

            Layout.fillHeight: true
            Layout.fillWidth: true

            Text {
                text: "Hello world? "
            }
            Text {
                text: "Hello world!"
            }

            Text {
                text: "Goodbye world? "
            }
            Text {
                text: "Goodbye world!"
            }

        }

        Item {
            width: 10
            height: 10
        }
    }

    Button {
        text: "Create Video"
        onClicked: {
            video.hidePanel();
            video.createVideo();
        }
    }
}

// Dialog {
//     id: video
//     // visible: true
//     title: "Videocapture"

//     standardButtons: StandardButton.Ok | StandardButton.Cancel

//     property string returnValue: ""

//     contentItem: Rectangle {
//         implicitWidth: 600
//         implicitHeight: 400


//         ColumnLayout {
//             anchors.fill: parent
//             anchors.margins: 10

//             ComboBox {
//                 id: comboBox
//                 currentIndex: 2
//                 activeFocusOnPress: true

//                 model: ListModel {
//                     id: cbItems
//                     ListElement { text: "Banana" }
//                     ListElement { text: "Apple" }
//                     ListElement { text: "Coconut" }
//                 }

//                 width: 200
//             }

//             Rectangle {
//                 height: 30
//                 Layout.fillWidth: true
//                 Button {
//                     text: "Ok"
//                     anchors.centerIn: parent
//                     onClicked: {
//                         returnValue = comboBox.currentText;
//                         video.close();
//                     }
//                 }
//             }
//         }
//     }
// }