
import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


Window
{
    id: pipelineSave

    property alias layout: mainLayout

    width:  600
    height: mainLayout.height + 2 * mainLayout.anchors.margins

    title: 'Save Pipeline'

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

                placeholderText:  "e.g. /home/user/pipelines/myPipeline.pipeline"
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

        Button
        {
            anchors.right: parent.right

            text: "Save Pipeline"

            onClicked:
            {
                gloperate.canvas0.pipeline.DemoPipeline.save(filepath.text);

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
