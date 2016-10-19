
import QtQuick 2.0
import QtQuick.Dialogs 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0


BaseItem
{
    id: item

    property var    pipelineInterface: null ///< Interface for communicating with the actual pipeline
    property string path:              ''   ///< Path to pipeline slot (e.g., 'pipeline.Stage1.in1')

    implicitWidth:  input.implicitWidth + button.implicitWidth + input.anchors.rightMargin
    implicitHeight: input.implicitHeight

    TextField
    {
        id: input

        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        anchors.left:   parent.left
        anchors.right:  button.left
        anchors.rightMargin: Ui.style.paddingMedium

        onEditingFinished:
        {
            pipelineInterface.setSlotValue(path, text);
        }
    }

    Button
    {
        id: button

        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        anchors.right:  parent.right
        implicitWidth:  32

        text: '...'

        onClicked:
        {
            fileDialog.open();
        }
    }

    FileDialog
    {
        id: fileDialog

        title:          'Open texture'
        selectExisting: true
        selectFolder:   false
        selectMultiple: false

        onAccepted:
        {
            var path = QmlUtils.urlToLocaFile(fileUrl);

            input.text = path;

            pipelineInterface.setSlotValue(path, path);
        }
    }

    function update()
    {
        var slotInfo = pipelineInterface.getSlot(path);
        input.text = slotInfo.value;
    }
}
