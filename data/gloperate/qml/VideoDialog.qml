import QtQuick 2.2
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import QtQml 2.2
import gloperate.base 1.0

Background {
    id: video

    signal close();

    property int margin: 0
    property alias layout: mainLayout

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin
        Controls.GroupBox {
            id: rowBox
            title: "Save as"
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                TextField {
                    id: filepath
                    placeholderText: "e.g. /home/user/videos/video.avi"
                    Layout.fillWidth: true
                }
                DialogButton {
                    text: "Browse"

                    onClicked: {
                        fileDialog.open();
                    }
                }
            }
        }

        Controls.GroupBox {
            title: "Settings"
            Layout.fillWidth: true

            GridLayout {
                id: gridLayout
                rows: 4
                flow: GridLayout.TopToBottom
                anchors.fill: parent

                Controls.Label { text: "Width" }
                Controls.Label { text: "Height" }
                Controls.Label { text: "FPS" }
                Controls.Label { text: "Duration (sec)" }

                ComboBox {
                    editable: true
                    id: width
                    model: ListModel {
                        ListElement { text: "800" }
                        ListElement { text: "1024" }
                        ListElement { text: "1152" }
                        ListElement { text: "1280" }
                        ListElement { text: "1360" }
                        ListElement { text: "1440" }
                        ListElement { text: "1600" }
                        ListElement { text: "1680" }
                        ListElement { text: "1920" }
                        ListElement { text: "2560" }
                    }
                }
                
                ComboBox {
                    editable: true
                    id: height
                    model: ListModel {
                        ListElement { text: "600" }
                        ListElement { text: "720" }
                        ListElement { text: "768" }
                        ListElement { text: "800" }
                        ListElement { text: "864" }
                        ListElement { text: "900" }
                        ListElement { text: "1024" }
                        ListElement { text: "1050" }
                        ListElement { text: "1200" }
                        ListElement { text: "1440" }
                        ListElement { text: "1600" }
                    }
                }

                ComboBox {
                    editable: true
                    id: fps
                    model: ListModel {
                        ListElement { text: "30" }
                        ListElement { text: "60" }
                    }
                }

                ComboBox {
                    editable: true
                    id: duration
                    model: ListModel {
                        ListElement { text: "5" }
                        ListElement { text: "10" }
                        ListElement { text: "60" }
                    }
                }

            }
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true

            minimumValue: 0
            maximumValue: 100
            value: 35
        }

        DialogButton {
            text: "Record Video"
            anchors.right: parent.right

            icon: '0021-video-camera.png'

            onClicked: {
                progressTimer.restart();

                gloperate.surface0.createVideo(filepath.text, width.editText, height.editText, fps.editText, duration.editText);
            }
        }
    }

    Timer {
        id: progressTimer

        interval: 50
        repeat: true
        triggeredOnStart: true

        property int progress: 0

        onTriggered: {
            progress = gloperate.surface0.exportProgress();
            progressBar.value = progress;
            print(progress);

            if (progress >= 100) {
                stop();
                close();
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose an export location and filename"
        folder: shortcuts.home
        selectFolder: false
        selectExisting: false
        selectMultiple: false

        onAccepted: {
            var path = fileUrl.toString();
            // remove prefixed "file:///"
            path = path.replace(/^(file:\/{3})/,"");
            // unescape html codes like '%23' for '#'
            path = decodeURIComponent(path);
            filepath.text = path;
            
            close();
        }
        onRejected: {
            close();
        }
    }
}
