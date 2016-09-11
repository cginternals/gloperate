
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

    property string path: '' ///< Path in the pipeline hierarchy (e.g., 'DemoPipeline.DemoStage')

    property string name:    'Stage'
    property int    radius:  Ui.style.pipelineConnectorSize

    property Item pipeline:  null
    property var  slotItems: null ///< Item cache

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

        // Reset item cache
        slotItems = {};
    }

    /**
    *  Load stage from pipeline viewer
    */
    onPathChanged:
    {
        clear();

        // Get pipeline container
        var pipelineContainer = gloperate.canvas0.pipeline;

        // Get stage
        var stage     = pipeline.getStage(path);
        var stageDesc = stage.getDescription();

        // Add parameters
        for (var i in stageDesc.parameters)
        {
            var paramName = stageDesc.parameters[i];
            var slotItem = inputSlotComponent.createObject(inputs, { name: paramName || 'Parameter', color: Ui.style.pipelineConnectorColorParam });
            slotItems[paramName] = slotItem;
        }

        // Add inputs
        for (var i in stageDesc.inputs)
        {
            var inputName = stageDesc.inputs[i];
            var slotItem = inputSlotComponent.createObject(inputs, { name: inputName || 'Input' });
            slotItems[inputName] = slotItem;
        }

        // Add outputs
        for (var i in stageDesc.outputs)
        {
            var outputName = stageDesc.outputs[i];
            var slotItem = outputSlotComponent.createObject(outputs, { name: outputName || 'Output' });
            slotItems[outputName] = slotItem;
        }

        // Add proxy outputs
        for (var i in stageDesc.proxyOutputs)
        {
            var proxyName = stageDesc.proxyOutputs[i];
            var slotItem = outputSlotComponent.createObject(outputs, { name: proxyName || 'Output' });
            slotItems[proxyName] = slotItem;
        }
    }

    function getSlotPos(name, type)
    {
        var slot = slotItems[name];

        if (slot)
        {
            if (type == 'input')
            {
                return slot.mapToItem(item, -slot.radius / 8.0, slot.radius / 2.0);
            }
            else if (type == 'output')
            {
                return slot.mapToItem(item, slot.width, slot.radius / 2.0);
            }
        }

        return null;
    }
}
