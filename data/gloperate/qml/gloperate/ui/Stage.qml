
import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

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

    signal closed()

    property string path: '' ///< Path in the pipeline hierarchy (e.g., 'DemoPipeline.DemoStage')

    property bool includeInputs:  true
    property bool includeOutputs: true
    property bool inverse:        false
    property bool allowClose:     true

    property string name:    'Stage'
    property int    radius:  Ui.style.pipelineConnectorSize
    property color  color:   Ui.style.pipelineTitleColor

    property Item pipeline:  null
    property var  slotItems: null ///< Item cache

    implicitWidth:  Math.max(connectors.implicitWidth, body.implicitWidth + radius)
    implicitHeight: connectors.implicitHeight + title.implicitHeight + Ui.style.paddingLarge

    Drag.active: mouseArea.drag.active

    onPathChanged:
    {
        update();
    }

    AddSlotDialog
    {
        id: dialog

        onCreateSlot:
        {
            pipeline.getStage(item.path).createSlot(slotType, type, name);
            item.update();
        }
    }

    Menu
    {
        id: menu

        title: "Stage"

        MenuItem
        {
            text: 'Add Input'

            onTriggered:
            {
                dialog.slotType = 'Input';
                dialog.setChoices( pipeline.getStage(item.path).slotTypes() );
                dialog.open();
            }
        }

        MenuItem
        {
            text: 'Add Parameter'

            onTriggered:
            {
                dialog.slotType = 'Parameter';
                dialog.open();
            }
        }

        MenuItem
        {
            text: 'Add Output'

            onTriggered:
            {
                dialog.slotType = 'Output';
                dialog.open();
            }
        }

        MenuItem
        {
            text: 'Add ProxyOutput'

            onTriggered:
            {
                dialog.slotType = 'ProxyOutput';
                dialog.open();
            }
        }
    }

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
            implicitWidth:   label.implicitWidth  + 3 * label.anchors.margins + icon.implicitWidth
            implicitHeight:  label.implicitHeight + 2 * label.anchors.margins

            radius: body.radius
            color:  item.color
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

                acceptedButtons: Qt.LeftButton | Qt.RightButton
                drag.target:     item

                onClicked:
                {
                    if (mouse.button == Qt.RightButton)
                    {
                        menu.popup();
                    }
                }
            }

            Icon
            {
                id: icon

                anchors.verticalCenter: label.verticalCenter
                anchors.right:          parent.right
                anchors.margins:        Ui.style.paddingMedium

                icon:       '0272-cross.png'
                iconWidth:  12
                iconHeight: 12

                visible: allowClose

                onClicked:
                {
                    item.closed();
                }
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
    function update()
    {
        clear();

        if (path == '')
            return;

        // Get pipeline container
        var pipelineContainer = gloperate.canvas0.pipeline;

        // Get stage
        var stage     = pipeline.getStage(path);
        var stageDesc = stage.getDescription();

        if (includeInputs)
        {
            var component  = inverse ? outputSlotComponent : inputSlotComponent;
            var parentItem = inverse ? outputs : inputs;

            // Add parameters
            for (var i in stageDesc.parameters)
            {
                var paramName = stageDesc.parameters[i];

                var slotItem = component.createObject(parentItem, {
                    pipeline: item.pipeline,
                    path: path + '.' + paramName,
                    name: paramName,
                    color: Ui.style.pipelineConnectorColorParam,
                    connectable: false
                } );

                slotItems[paramName] = slotItem;
            }

            // Add inputs
            for (var i in stageDesc.inputs)
            {
                var inputName = stageDesc.inputs[i];

                var slotItem = component.createObject(parentItem, {
                    pipeline: item.pipeline,
                    path: path + '.' + inputName,
                    name: inputName
                } );

                slotItems[inputName] = slotItem;
            }
        }

        if (includeOutputs)
        {
            var component  = inverse ? inputSlotComponent : outputSlotComponent;
            var parentItem = inverse ? inputs : outputs;

            // Add outputs
            for (var i in stageDesc.outputs)
            {
                var outputName = stageDesc.outputs[i];

                var slotItem = component.createObject(parentItem, {
                    pipeline: item.pipeline,
                    path: path + '.' + outputName,
                    name: outputName
                } );

                slotItems[outputName] = slotItem;
            }

            // Add proxy outputs
            for (var i in stageDesc.proxyOutputs)
            {
                var proxyName = stageDesc.proxyOutputs[i];

                var slotItem = component.createObject(parentItem, {
                    pipeline: item.pipeline,
                    path: path + '.' + proxyName,
                    name: proxyName
                } );

                slotItems[proxyName] = slotItem;
            }
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
