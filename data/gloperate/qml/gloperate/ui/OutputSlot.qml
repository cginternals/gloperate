
import QtQuick 2.0
import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  OutputSlot
*
*  Representation of an output slot
*/
BaseItem
{
    id: item

    implicitWidth:  row.implicitWidth  + 2 * row.anchors.margins
    implicitHeight: row.implicitHeight + 2 * row.anchors.margins

    property string path:        '' ///< Path in the pipeline hierarchy (e.g., 'DemoPipeline.DemoStage.texture')
    property alias  name:        label.text
    property int    radius:      Ui.style.pipelineConnectorSize
    property color  color:       Ui.style.pipelineConnectorColorOut
    property color  borderColor: Ui.style.pipelineLineColor
    property int    borderWidth: Ui.style.pipelineLineWidth

    property Item pipeline: null
    readonly property bool hovered:  (pipeline != null && pipeline.hoveredElement  == path)
    readonly property bool selected: (pipeline != null && pipeline.selectedElement == path)

    anchors.right: parent.right

    Row
    {
        id: row

        anchors.left: parent.left
        anchors.top:  parent.top
        spacing:      Ui.style.paddingSmall

        Label
        {
            id: label

            anchors.verticalCenter: parent.verticalCenter

            text:  'Value'
            color: Ui.style.pipelineTextColor
        }

        Rectangle
        {
            id: connector

            anchors.verticalCenter: parent.verticalCenter
            width:                  item.radius
            height:                 item.radius

            radius:       width / 2.0
            color:        item.selected ? Ui.style.pipelineColorSelected : (item.hovered ? Ui.style.pipelineColorHighlighted : item.color)
            border.color: item.borderColor
            border.width: item.borderWidth
        }
    }

    MouseArea
    {
        anchors.fill: parent

        hoverEnabled: true

        onEntered:
        {
            pipeline.elementEntered(item.path);
        }

        onExited:
        {
            pipeline.elementLeft(item.path);
        }

        onPressed:
        {
            pipeline.selectOutput(item.path);
        }
    }
}
