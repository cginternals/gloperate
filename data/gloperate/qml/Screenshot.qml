import QtQuick 2.2
import QtQuick.Controls 1.2 as Controls
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
// import gloperate.ui 1.0

// Background {
//     id: screenshot

//     property int margin: 0
//     property alias layout: mainLayout
Controls.ApplicationWindow {
    id: screenshot
    visible: true
    property int margin: 11
    width: mainLayout.implicitWidth + 2 * margin
    height: mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

    signal fileChosen

    onFileChosen: {
        var path = fileDialog.fileUrl.toString();
        // remove prefixed "file:///"
        path = path.replace(/^(file:\/{3})/,"");
        // unescape html codes like '%23' for '#'
        path = decodeURIComponent(path);
        filepath.text = path;
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
                Controls.TextField {
                    id: filepath
                    placeholderText: "e.g. /home/user/images/screen.jpg"
                    Layout.fillWidth: true
                    // style: TextFieldStyle {
                    //         textColor: Ui.style.textColor
                    //         background: Rectangle {
                    //             color: Ui.style.backgroundColor
                    //             radius: 2
                    //             implicitWidth: 100
                    //             implicitHeight: 24
                    //             // border.color: "#333"
                    //             border.width: 1
                    //         }
                    //     }
                }
                Controls.Button {
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

                Controls.ComboBox {
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
                
                Controls.ComboBox {
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

                Controls.ComboBox {
                    editable: true
                    id: iterations
                    model: ListModel {
                        ListElement { text: "1" }
                        ListElement { text: "2" }
                        ListElement { text: "4" }
                        ListElement { text: "8" }
                        ListElement { text: "16" }
                    }
                }

            }
        }

        Controls.Button {
            text: "Take Screenshot"
            anchors.right: parent.right

            onClicked: {
                // Validate filename
                console.log(filepath.text);
                // Validate that the rest are numbers
                console.log(width.editText);
                console.log(height.editText);
                console.log(iterations.editText);
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
            close();
            screenshot.fileChosen();
        }
        onRejected: {
            close();
        }
    }
}
