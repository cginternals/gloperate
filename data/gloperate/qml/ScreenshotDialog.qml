
import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


Window
{
    id: screenshot

    property alias layout: mainLayout

    width:  600
    height: mainLayout.height + 2 * mainLayout.anchors.margins

    title: 'Save Screenshot'

    Column
    {
        id: mainLayout

        anchors.top:     parent.top
        anchors.left:    parent.left
        anchors.right:   parent.right
        anchors.margins: Ui.style.paddingMedium

        spacing: Ui.style.spacingMedium

        GroupBox
        {
            id: rowBox

            width: parent.width

            title: "Save as"

            TextField
            {
                id: filepath

                anchors.left:    parent.left
                anchors.right:   button.left
                anchors.margins: Ui.style.paddingMedium

                placeholderText:  "e.g. /home/user/images/screen.jpg"
            }

            Button
            {
                id: button

                anchors.right: parent.right

                text: "Browse"

                onClicked:
                {
                    fileDialog.open();
                }
            }
        }

        GroupBox
        {
            width: parent.width

            title: "Settings"

            GridLayout
            {
                id: gridLayout

                width: parent.width

                rows: 3
                flow: GridLayout.TopToBottom

                Label
                {
                    text: "Width"
                }

                Label
                {
                    text: "Height"
                }

                Label
                {
                    text: "Renderiterations"
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
                    id: iterations

                    editable: true

                    model: ListModel
                    {
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

        Button
        {
            anchors.right: parent.right

            text: "Take Screenshot"

            onClicked:
            {
                gloperate.canvas0.exportImage(filepath.text, width.editText, height.editText, iterations.editText);

                close();
            }
        }
    }

    FileDialog
    {
        id: fileDialog

        title:          "Please choose an export location and filename"
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
}
