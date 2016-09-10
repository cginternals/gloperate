
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

    property real nx: x + inputs.children[0].mapToItem(item, 0, 0).x
    property real ny: y + inputs.children[0].mapToItem(item, 0, 0).y

    Drag.active: mouseArea.drag.active

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

                text: item.name
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

            InputSlot { radius: item.radius; text: 'Value1' }
            InputSlot { radius: item.radius; text: 'Value2' }
            InputSlot { radius: item.radius; text: 'Value3' }
            InputSlot { radius: item.radius; text: 'NochnValue' }
        }

        Column
        {
            id: outputs

            anchors.right: parent.right
            spacing:       Ui.style.paddingSmall

            OutputSlot { radius: item.radius; text: 'Out1' }
            OutputSlot { radius: item.radius; text: 'Out2' }
            OutputSlot { radius: item.radius; text: 'Out3' }
        }
    }
}
