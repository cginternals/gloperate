
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

    property Component stageDelegate: null
    property string    targetStage:   ''

    property string name:      ''
    property var inputs:       []
    property var outputs:      []

    implicitWidth:  inputs.implicitWidth + outputs.implicitWidth + pipeline.implicitWidth + 2 * Ui.style.panelPadding
    implicitHeight: label.implicitHeight + Math.max(Math.max(inputs.implicitHeight, outputs.implicitHeight), pipeline.implicitHeight) + 6 * Ui.style.panelPadding

    clip: true

    Rectangle
    {
        id: body

        anchors.top:     parent.top
        anchors.bottom:  parent.bottom
        anchors.left:    inputs.horizontalCenter
        anchors.right:   outputs.horizontalCenter
        anchors.margins: Ui.style.panelPadding

        color:        Ui.style.pipelineStageColor
        radius:       Ui.style.pipelineStageRadius
        border.color: Ui.style.pipelineLineColor
        border.width: Ui.style.pipelineLineWidth

        z: 0
    }

    Label
    {
        id: label

        anchors.horizontalCenter: body.horizontalCenter
        anchors.top:              body.top
        anchors.topMargin:        Ui.style.panelPadding

        text:  item.name
        color: Ui.style.pipelineTextColor

        z: 1
    }

    ColumnLayout
    {
        id: inputs

        anchors.top:       label.bottom
        anchors.left:      parent.left
        anchors.topMargin: Ui.style.panelPadding

        spacing: Ui.style.ctrlSpacing

        z: 1

        Repeater
        {
            model: item.inputs.length

            delegate: Slot
            {
                Layout.fillWidth: true

                name:      item.inputs[index].name
                value:     item.inputs[index].value
                switched:  true
                hasInput:  !item.inputs[index].hasOwnData
                hasOutput: true
                showValue: item.inputs[index].hasOwnData
            }
        }
    }

    Item
    {
        id: pipeline

        anchors.left:    inputs.right
        anchors.top:     label.bottom
        anchors.margins: Ui.style.panelPadding
        implicitWidth:   Math.max(realPipeline.implicitWidth,  label.implicitWidth)
        implicitHeight:  Math.max(realPipeline.implicitHeight, label.implicitHeight)

        Pipeline
        {
            id: realPipeline

            anchors.fill: parent

            stageDelegate: item.stageDelegate
            targetStage:   item.targetStage
        }
    }

    ColumnLayout
    {
        id: outputs

        anchors.top:     label.bottom
        anchors.left:    pipeline.right
        anchors.margins: Ui.style.panelPadding

        z:       1
        spacing: Ui.style.ctrlSpacing

        Repeater
        {
            model: item.outputs.length

            delegate: Slot
            {
                Layout.fillWidth: true

                name:      item.outputs[index].name
                value:     item.outputs[index].value
                hasInput:  !item.outputs[index].hasOwnData
                hasOutput: true
                showValue: item.outputs[index].hasOwnData
            }
        }
    }

    onTargetStageChanged:
    {
        update();
    }

    function update()
    {
        // Update stage name
        item.name = gloperate.pipeline.getName(item.targetStage);

        // List inputs
        var inputs = [];

        var inputNames = gloperate.pipeline.getInputs(item.targetStage);
        for (var i=0; i<inputNames.length; i++)
        {
            var name = inputNames[i];

            inputs.push({
                name:       name,
                value:      gloperate.pipeline.getValue(item.targetStage + '.' + name),
                hasOwnData: false
            });
        }

        var parameterNames = gloperate.pipeline.getParameters(item.targetStage);
        for (var i=0; i<parameterNames.length; i++)
        {
            var name = parameterNames[i];

            inputs.push({
                name:      name,
                value:     gloperate.pipeline.getValue(item.targetStage + '.' + name),
                hasOwnData: true
            });
        }

        item.inputs = inputs;

        // List outputs
        var outputs = [];

        var outputNames = gloperate.pipeline.getOutputs(item.targetStage);
        for (var i=0; i<outputNames.length; i++)
        {
            var name = outputNames[i];

            outputs.push({
                name:       name,
                value:      gloperate.pipeline.getValue(item.targetStage + '.' + name),
                hasOwnData: true
            });
        }

        var proxyOutputNames = gloperate.pipeline.getProxyOutputs(item.targetStage);
        for (var i=0; i<proxyOutputNames.length; i++)
        {
            var name = proxyOutputNames[i];

            outputs.push({
                name:       name,
                value:      gloperate.pipeline.getValue(item.targetStage + '.' + name),
                hasOwnData: false
            });
        }

        item.outputs = outputs;
    }
}
