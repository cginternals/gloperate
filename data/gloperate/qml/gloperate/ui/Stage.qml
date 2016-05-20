
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
    property var inputNames:   []
    property var inputValues:  []
    property var outputNames:  []
    property var outputValues: []

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
            model: item.inputNames.length

            delegate: Slot
            {
                Layout.fillWidth: true

                name:     item.inputNames[index]
                value:    item.inputValues[index]
                switched: true
            }
        }
    }

    Pipeline
    {
        id: pipeline

        anchors.left:    inputs.right
        anchors.top:     label.bottom
        anchors.margins: Ui.style.panelPadding

        stageDelegate: item.stageDelegate
        targetStage:   item.targetStage
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
            model: item.outputNames.length

            delegate: Slot
            {
                Layout.fillWidth: true

                name:  item.outputNames[index]
                value: item.outputValues[index]
            }
        }
    }

    onTargetStageChanged:
    {
        item.name        = gloperate.pipeline.getName(item.targetStage);
        item.inputNames  = gloperate.pipeline.getInputs(item.targetStage);
        item.outputNames = gloperate.pipeline.getOutputs(item.targetStage);

        var inputValues = [];
        for (var i=0; i<item.inputNames.length; i++) {
            inputValues.push(
                gloperate.pipeline.getValue(item.targetStage + '.' + item.inputNames[i])
            );
        }
        item.inputValues = inputValues;

        var outputValues = [];
        for (var i=0; i<item.outputNames.length; i++) {
            outputValues.push(
                gloperate.pipeline.getValue(item.targetStage + '.' + item.outputNames[i])
            );
        }
        item.outputValues = outputValues;
    }
}
