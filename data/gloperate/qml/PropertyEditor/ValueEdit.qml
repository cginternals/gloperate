
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0


Item
{
    id: item

    property var    pipelineInterface: null ///< Interface for communicating with the actual pipeline
    property string path:              ''   ///< Path to pipeline slot (e.g., 'pipeline.Stage1.in1')

    implicitWidth:  input ? input.implicitWidth  : 0
    implicitHeight: input ? input.implicitHeight : 0

    property Item   input:     null
    property string inputType: ''

    function update()
    {
        // Get slot type
        var slotInfo = pipelineInterface.getSlot(path);
        if (!slotInfo.type || slotInfo.type == '')
            return;

        // Need to (re-)create editor?
        if (input == null || inputType != slotInfo.type)
        {
            // Destroy old editor
            if (input != null)
            {
                input.destroy();
            }

            // Create editor
            inputType = slotInfo.type;
            if (slotInfo.type == 'bool')
                input = editBool.createObject(item);
            else if (slotInfo.type == 'int' || slotInfo.type == 'float')
                input = editNumber.createObject(item);
            else if (slotInfo.type == 'color')
                input = editColor.createObject(item);
            else if (slotInfo.type == 'filename')
                input = editFilename.createObject(item);
            else
                input = editString.createObject(item);
        }

        // Update editor
        input.update();
    }

    Component
    {
        id: editString

        ValueEditString
        {
            anchors.fill: parent

            pipelineInterface: item.pipelineInterface
            path:              item.path
        }
    }

    Component
    {
        id: editFilename

        ValueEditFilename
        {
            anchors.fill: parent

            pipelineInterface: item.pipelineInterface
            path:              item.path
        }
    }

    Component
    {
        id: editBool

        ValueEditBool
        {
            anchors.fill: parent

            pipelineInterface: item.pipelineInterface
            path:              item.path
        }
    }

    Component
    {
        id: editNumber

        ValueEditNumber
        {
            anchors.fill: parent

            pipelineInterface: item.pipelineInterface
            path:              item.path
        }
    }

    Component
    {
        id: editColor

        ValueEditColor
        {
            anchors.fill: parent

            pipelineInterface: item.pipelineInterface
            path:              item.path
        }
    }
}
