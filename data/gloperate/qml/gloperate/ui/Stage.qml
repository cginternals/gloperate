
import QtQuick 2.0
import QtQuick.Layouts 1.0

import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  Stage
*
*  Representation of a stage in a pipeline
*/
BaseItem
{
    id: item

    property string name:    'Stage'
    property int    radius:  Ui.style.pipelineConnectorSize

    implicitWidth:  Math.max(connectors.implicitWidth, body.implicitWidth + radius)
    implicitHeight: connectors.implicitHeight + title.implicitHeight + Ui.style.paddingLarge

    Drag.active: mouseArea.drag.active

    Rectangle
    {
        id: body

        anchors.fill:        parent
        anchors.leftMargin:  item.radius / 2
        anchors.rightMargin: item.radius / 2
        implicitWidth:       title.implicitWidth + 2 * title.anchors.margins

        color:        Ui.style.pipelineStageColor
        border.color: Ui.style.pipelineLineColor
        border.width: Ui.style.pipelineLineWidth
        radius:       Ui.style.pipelineStageRadius

        Rectangle
        {
            id: title

            anchors.top:     parent.top
            anchors.left:    parent.left
            anchors.right:   parent.right
            anchors.margins: body.border.width
            implicitWidth:   label.implicitWidth  + 2 * label.anchors.margins
            implicitHeight:  label.implicitHeight + 2 * label.anchors.margins

            radius: body.radius
            color:  Ui.style.pipelineTitleColor
            clip:   true

            Rectangle
            {
                anchors.left:   parent.left
                anchors.right:  parent.right
                anchors.bottom: parent.bottom
                height:         parent.height / 2
                color:          parent.color
            }

            Label
            {
                id: label

                anchors.fill:    parent
                anchors.margins: Ui.style.paddingMedium

                text:  item.name
                color: Ui.style.pipelineTitleTextColor
            }

            MouseArea
            {
                id: mouseArea

                anchors.fill: parent

                acceptedButtons: Qt.LeftButton
                drag.target:     item
            }
        }
    }

    Item
    {
        id: connectors

        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.bottom: parent.bottom
        implicitWidth:  inputs.implicitWidth + Ui.style.paddingLarge + outputs.implicitWidth
        implicitHeight: Math.max(inputs.implicitHeight, outputs.implicitHeight) + Ui.style.paddingLarge

        Column
        {
            id: inputs

            anchors.left: parent.left
            spacing:      Ui.style.paddingSmall
        }

        Column
        {
            id: outputs

            anchors.right: parent.right
            spacing:       Ui.style.paddingSmall
        }
    }

    /**
    *  Component that contains the template for an input slot
    */
    property Component inputSlotComponent: InputSlot
    {
    }

    /**
    *  Component that contains the template for an output slot
    */
    property Component outputSlotComponent: OutputSlot
    {
    }

    /**
    *  Clear stage
    */
    function clear()
    {
        // Destroy all inputs
        for (var i in inputs.children)
        {
            var slot = inputs.children[i];
            slot.destroy();
        }

        // Destroy all outputs
        for (var i in outputs.children)
        {
            var slot = outputs.children[i];
            slot.destroy();
        }
    }

    /**
    *  Load stage from pipeline viewer
    *
    *  @param[in] path
    *    Path in the pipeline hierarchy (e.g., 'DemoPipeline.SubPipeline')
    */
    function load(path)
    {
        // Get pipeline container
        var pipelineContainer = gloperate.canvas0.pipeline;

        // Get stage
        var stage     = getStage(path);
        var stageDesc = stage.getDescription();

        // Add parameters
        for (var i in stageDesc.parameters)
        {
            var paramName = stageDesc.parameters[i];
            inputSlotComponent.createObject(inputs, { text: paramName || 'Parameter', color: Ui.style.pipelineConnectorColorParam });
        }

        // Add inputs
        for (var i in stageDesc.inputs)
        {
            var inputName = stageDesc.inputs[i];
            inputSlotComponent.createObject(inputs, { text: inputName || 'Input' });
        }

        // Add outputs
        for (var i in stageDesc.outputs)
        {
            var outputName = stageDesc.outputs[i];
            outputSlotComponent.createObject(outputs, { text: outputName || 'Output' });
        }

        // Add proxy outputs
        for (var i in stageDesc.proxyOutputs)
        {
            var proxyName = stageDesc.proxyOutputs[i];
            outputSlotComponent.createObject(outputs, { text: proxyName || 'Output' });
        }
    }

    function getInputPos(index, x, y)
    {
        var inputSlot = inputs.children[index];

        var pos = inputSlot.mapToItem(item, x, y);
        pos.x -= inputSlot.radius / 8.0;
        pos.y += inputSlot.radius / 2.0;

        return pos;
    }

    function getOutputPos(index, x, y)
    {
        var outputSlot = outputs.children[index];

        var pos = outputSlot.mapToItem(item, x, y);
        pos.x += outputSlot.width;
        pos.y += outputSlot.radius / 2.0;

        return pos;
    }

    function getStage(path)
    {
        var stage = gloperate.canvas0.pipeline;

        var names = path.split('.');
        for (var i=0; i<names.length; i++)
        {
            var name = names[i];
            stage = stage[name];
        }

        return stage;
    }
}
