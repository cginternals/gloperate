
import QtQuick 2.0
import QtQuick.Controls 1.0 as Controls
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import gloperate.base 1.0


Background {
    id: screenshot

    signal close()

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
                    placeholderText: "e.g. /home/user/images/screen.jpg"
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
                rows: 3
                flow: GridLayout.TopToBottom
                anchors.fill: parent

                Controls.Label { text: "Width" }
                Controls.Label { text: "Height" }
                Controls.Label { text: "Renderiterations" }

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
                    id: iterations
                    model: ListModel {
                        ListElement { text: "1" }
                        ListElement { text: "2" }
                        ListElement { text: "4" }
                        ListElement { text: "8" }
                        ListElement { text: "16" }
                        ListElement { text: "32" }
                        ListElement { text: "64" }
                        ListElement { text: "128" }
                    }
                }

            }
        }

        DialogButton {
            text: "Take Screenshot"
            anchors.right: parent.right

            icon: '0040-file-picture.png'

            onClicked: {
                gloperate.canvas0.exportImage(filepath.text, width.editText, height.editText, iterations.editText);

                close();
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose an export location and filename"
//      folder: shortcuts.home
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
