
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

    implicitWidth:  150
    implicitHeight: 25

    Rectangle
    {
        id: input

        anchors.fill: parent

        color: '#000000'

        MouseArea
        {
            anchors.fill: parent

            onClicked:
            {
                colorDialog.color = input.color;
                colorDialog.open();
            }
        }
    }

    ColorDialog
    {
        id: colorDialog

        onAccepted:
        {
            input.color = color;

            pipelineInterface.setSlotValue(path, color);
        }
    }

    function update()
    {
        var slotInfo = pipelineInterface.getSlot(path);
        input.color = slotInfo.value;
    }
}
