
import QtQuick 2.0
import QtQuick.Controls 1.0 as Controls
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQml 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0

import gloperate.base 1.0


Window
{
    id: video

    signal closed()

    property int   margin: 0
    property alias layout: mainLayout

    width:  600
    height: mainLayout.height + 2 * mainLayout.anchors.margins

    title: 'Export Video'

    ColumnLayout 
    {
        id: mainLayout

        anchors.top:     parent.top
        anchors.left:    parent.left
        anchors.right:   parent.right
        anchors.margins: Ui.style.paddingMedium

        spacing: Ui.style.spacingMedium

        Controls.GroupBox
        {
            id: rowBox

            title: "Save as"

            Layout.fillWidth: true

            RowLayout
            {
                anchors.fill: parent

                TextField
                {
                    id: filepath

                    placeholderText: "e.g. /home/user/videos/video.avi"

                    Layout.fillWidth: true
                }

                Button
                {
                    text: "Browse"

                    onClicked:
                    {
                        fileDialog.open();
                    }
                }
            }
        }

        Controls.GroupBox
        {
            title: "Settings"

            Layout.fillWidth: true

            GridLayout
            {
                id: gridLayout

                anchors.fill: parent

                rows: 6
                flow: GridLayout.TopToBottom

                Controls.Label { text: "Profile" }
                Controls.Label { text: "Width" }
                Controls.Label { text: "Height" }
                Controls.Label { text: "FPS" }
                Controls.Label { text: "Duration (sec)" }
                Controls.Label { text: "Backend" }

                ComboBox
                {
                    id: profile

                    property alias currentProfile: profile.currentIndex

                    Layout.fillWidth: true

                    editable: false

                    model: ListModel
                    {
                        id: profiles
                    }

                    onCurrentProfileChanged:
                    {
                        videoProfile.profileIndex = currentProfile;
                    }
                }

                ComboBox
                {
                    id: width

                    editable: true

                    model: ListModel
                    {
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
                
                ComboBox
                {
                    id: height

                    editable: true

                    model: ListModel
                    {
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

                ComboBox
                {
                    id: fps

                    editable: true

                    model: ListModel
                    {
                        ListElement { text: "30" }
                        ListElement { text: "60" }
                    }
                }

                ComboBox {
                    id: duration

                    editable: true

                    model: ListModel
                    {
                        ListElement { text: "5" }
                        ListElement { text: "10" }
                        ListElement { text: "60" }
                    }
                }

                ComboBox {
                    id: backend

                    Layout.fillWidth: true

                    editable: false

                    model: ListModel
                    {
                        id: backends
                    }
                }
            }
        }

        ProgressBar
        {
            id: progressBar

            Layout.fillWidth: true

            minimumValue: 0
            maximumValue: 100
            value:        0
        }

        Item
        {
            Layout.fillWidth: true

            implicitHeight: Math.max(button1.implicitHeight, button2.implicitHeight)

            Button
            {
                id: button1

                anchors.left: parent.left

                text: 'Apply'

                onClicked:
                {
                    var parameters =
                    {
                        filepath: filepath.text,
                        width: width.editText,
                        height: height.editText,
                        fps: fps.editText,
                        duration: duration.editText,

                        format: videoProfile.format,
                        codec: videoProfile.codec,
                        gopsize: videoProfile.gopsize,
                        bitrate: videoProfile.bitrate
                    };

                    gloperate.canvas0.setVideoTarget(parameters, backend.editText);
                    // gloperate.canvas0.toggleVideoExport();

                    closed();
                    close();
                }
            }

            Button
            {
                id: button2

                anchors.right: parent.right

                text: 'Apply and Record'

                onClicked:
                {
                    var parameters =
                    {
                        filepath: filepath.text,
                        width: width.editText,
                        height: height.editText,
                        fps: fps.editText,
                        duration: duration.editText,

                        format: videoProfile.format,
                        codec: videoProfile.codec,
                        gopsize: videoProfile.gopsize,
                        bitrate: videoProfile.bitrate
                    };

                    gloperate.canvas0.exportVideo(parameters, backend.editText);

                    closed();
                    close();
                }
            }
        }
    }

    FileDialog
    {
        id: fileDialog

        title:          'Please choose an export location and filename'
        selectFolder:   false
        selectExisting: false
        selectMultiple: false

        onAccepted:
        {
            var path = QmlUtils.urlToLocaFile(fileUrl);

            filepath.text = path;
            
            close();
        }

        onRejected:
        {
            close();
        }
    }

    VideoProfile
    {
        id: videoProfile

        onProfileChanged:
        {
            updateParameters();
        }
    }

    function update()
    {
        backends.clear();
        profiles.clear();

        var plugins = gloperate.canvas0.videoExporterPlugins();
        plugins.forEach(function(backend) {
            backends.append({text: backend});
        });

        videoProfile.profiles.forEach(function(profile) {
            profiles.append({text: profile})
        });

        updateParameters();
    }

    function updateParameters()
    {
        width.editText    = videoProfile.width;
        height.editText   = videoProfile.height;
        fps.editText      = videoProfile.fps;
        duration.editText = videoProfile.seconds;
    }
}
