
import QtQuick 2.0
import QtQuick.Controls 1.0 as Controls
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQml 2.0
import gloperate.base 1.0


Background {
    id: video

    signal close();

    property int margin: 0
    property alias layout: mainLayout

    function update() {
        backends.clear();
        profiles.clear();

        var plugins = gloperate.canvas0.videoExporterPlugins();
        plugins.forEach(function(backend) {
            backends.append({text: backend});
        })

        videoProfile.profiles.forEach(function(profile) {
            profiles.append({text: profile})
        })

        updateParameters();
    }

    function updateParameters() {
        width.editText = videoProfile.width;
        height.editText = videoProfile.height;
        fps.editText = videoProfile.fps;
        duration.editText = videoProfile.seconds;
    }

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
                rows: 6
                flow: GridLayout.TopToBottom
                anchors.fill: parent

                Controls.Label { text: "Profile" }
                Controls.Label { text: "Width" }
                Controls.Label { text: "Height" }
                Controls.Label { text: "FPS" }
                Controls.Label { text: "Duration (sec)" }
                Controls.Label { text: "Backend" }

                ComboBox {
                    property alias currentProfile: profile.currentIndex

                    Layout.fillWidth: true
                    editable: false
                    id: profile
                    model: ListModel {
                        id: profiles
                    }

                    onCurrentProfileChanged: {
                        videoProfile.profileIndex = currentProfile;
                    }
                }

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

                ComboBox {
                    Layout.fillWidth: true
                    editable: false
                    id: backend
                    model: ListModel {
                        id: backends
                    }
                }

            }
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true

            minimumValue: 0
            maximumValue: 100
            value: 0
        }

        Controls.GroupBox {
            Layout.fillWidth: true

            GridLayout {
                anchors.fill: parent

                DialogButton {
                    text: "Apply and Record"
                    anchors.right: parent.right

                    icon: '0021-video-camera.png'

                    onClicked: {
                        var parameters = {
                            filepath: filepath.text,
                            width: width.editText,
                            height: height.editText,
                            fps: fps.editText,
                            duration: duration.editText,

                            format: videoProfile.format,
                            codec: videoProfile.codec,
                            gopsize: videoProfile.gopsize,
                            bitrate: videoProfile.bitrate
                        }

                        gloperate.canvas0.exportVideo(parameters, backend.editText);
                        close();
                    }
                }

                DialogButton {
                    id: asyncButton
                    text: "Apply"
                    anchors.left: parent.left

                    icon: '0021-video-camera.png'

                    onClicked: {
                        var parameters = {
                            filepath: filepath.text,
                            width: width.editText,
                            height: height.editText,
                            fps: fps.editText,
                            duration: duration.editText,

                            format: videoProfile.format,
                            codec: videoProfile.codec,
                            gopsize: videoProfile.gopsize,
                            bitrate: videoProfile.bitrate
                        }

                        gloperate.canvas0.setVideoTarget(parameters, backend.editText);
                        // gloperate.canvas0.toggleVideoExport();
                        close();
                    }
                }

                // DialogButton {
                //     text: "Toggle Off"
                //     anchors.left: asyncButton.right

                //     icon: '0021-video-camera.png'

                //     onClicked: {
                //         gloperate.canvas0.toggleVideoExport();
                //         close();
                //     }
                // }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose an export location and filename"
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

    VideoProfile {
        id: videoProfile

        onProfileChanged: {
            updateParameters();
        }
    }
}
