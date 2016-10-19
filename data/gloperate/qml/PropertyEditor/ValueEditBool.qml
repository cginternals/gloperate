
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0


BaseItem
{
    id: item

    property var    pipelineInterface: null ///< Interface for communicating with the actual pipeline
    property string path:              ''   ///< Path to pipeline slot (e.g., 'pipeline.Stage1.in1')

    implicitWidth:  input.implicitWidth
    implicitHeight: input.implicitHeight

    CheckBox
    {
        id: input

        anchors.fill: parent

        onClicked:
        {
            pipelineInterface.setSlotValue(path, checked);
        }
    }

    function update()
    {
        var slotInfo = pipelineInterface.getSlot(path);
        input.checked = (slotInfo.value == 'true' || slotInfo.value == true);
    }
}
