
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

    property string source: ''

    property string name:     ''
    property var inputNames:  []
    property var outputNames: []

    implicitWidth:  inputs.implicitWidth + body.implicitWidth + outputs.implicitWidth
    implicitHeight: Math.max(Math.max(inputs.implicitHeight, body.implicitHeight), outputs.implicitHeight)

    clip: true

    ColumnLayout
    {
        id: inputs

        anchors.top:  parent.top
        anchors.left: parent.left
        spacing:      Ui.style.ctrlSpacing

        Repeater
        {
            model: item.inputNames.length

            delegate: StageInput
            {
                Layout.fillWidth: true

                name: item.inputNames[index]
            }
        }
    }

    Rectangle
    {
        id: body

        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        anchors.left:   inputs.right
        anchors.right:  outputs.left
        implicitWidth:  label.implicitWidth  + 2 * Ui.style.panelPadding
        implicitHeight: label.implicitHeight + 2 * Ui.style.panelPadding

        color:        Ui.style.pipelineStageColor
        radius:       Ui.style.pipelineStageRadius
        border.color: Ui.style.pipelineLineColor
        border.width: Ui.style.pipelineLineWidth

        Label
        {
            id: label

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:   parent.verticalCenter

            text:  item.name
            color: 'black'
        }
    }

    ColumnLayout
    {
        id: outputs

        anchors.top:   parent.top
        anchors.right: parent.right
        spacing:       Ui.style.ctrlSpacing

        Repeater
        {
            model: item.outputNames.length

            delegate: StageOutput
            {
                Layout.fillWidth: true

                name: item.outputNames[index]
            }
        }
    }

    onSourceChanged:
    {
        item.name        = gloperate.pipeline.getName(item.source);
        item.inputNames  = gloperate.pipeline.getInputs(item.source);
        item.outputNames = gloperate.pipeline.getOutputs(item.source);
    }
}
