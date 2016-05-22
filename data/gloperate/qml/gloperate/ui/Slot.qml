
import QtQuick 2.0
import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  Slot
*
*  Representation of a slot (e.g., input or output)
*/
BaseItem
{
    id: item

    property string name:      'Slot'
    property string value:     ''
    property bool   switched:  false
    property bool   hasInput:  true
    property bool   hasOutput: true
    property bool   showValue: false

    implicitWidth:  body.implicitWidth + Ui.style.pipelineConnectorSize
    implicitHeight: body.implicitHeight

    Rectangle
    {
        id: connectorIn

        anchors.left:           parent.left
        anchors.verticalCenter: parent.verticalCenter
        width:                  Ui.style.pipelineConnectorSize
        height:                 Ui.style.pipelineConnectorSize

        visible:      item.hasInput
        color:        Ui.style.pipelineConnectorColorIn
        radius:       Ui.style.pipelineConnectorSize / 2.0
        border.color: Ui.style.pipelineLineColor
        border.width: Ui.style.pipelineLineWidth
        z:            1
    }

    Rectangle
    {
        id: body

        anchors.left:   connectorIn.horizontalCenter
        anchors.right:  connectorOut.horizontalCenter
        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        z:              0

        implicitWidth:  labelName.implicitWidth + (labelValue.visible ? labelValue.implicitWidth : 0) + Ui.style.pipelineConnectorSize + 4 * Ui.style.ctrlPadding
        implicitHeight: Ui.style.pipelineSlotSize

        color:        Ui.style.pipelineSlotColor
        radius:       Ui.style.pipelineStageRadius
        border.color: Ui.style.pipelineLineColor
        border.width: Ui.style.pipelineLineWidth

        Label
        {
            id: labelName

            anchors.verticalCenter: parent.verticalCenter
            anchors.left:           item.switched ? undefined : parent.left
            anchors.right:          item.switched ? parent.right : undefined
            anchors.leftMargin:     Ui.style.ctrlPadding + Ui.style.pipelineConnectorSize * 0.5
            anchors.rightMargin:    Ui.style.ctrlPadding + Ui.style.pipelineConnectorSize * 0.5

            text:  item.name
            color: Ui.style.pipelineTextColor
        }

        Label
        {
            id: labelValue

            anchors.verticalCenter: parent.verticalCenter
            anchors.left:           item.switched ? parent.left : undefined
            anchors.right:          item.switched ? undefined : parent.right
            anchors.leftMargin:     Ui.style.ctrlPadding + Ui.style.pipelineConnectorSize * 0.5
            anchors.rightMargin:    Ui.style.ctrlPadding + Ui.style.pipelineConnectorSize * 0.5

            visible: item.showValue
            text:    item.value
            color:   Ui.style.pipelineSecondaryTextColor
        }
    }

    Rectangle
    {
        id: connectorOut

        anchors.right:          parent.right
        anchors.verticalCenter: parent.verticalCenter
        width:                  Ui.style.pipelineConnectorSize
        height:                 Ui.style.pipelineConnectorSize
        z:                      1

        visible:      item.hasOutput
        color:        Ui.style.pipelineConnectorColorOut
        radius:       Ui.style.pipelineConnectorSize / 2.0
        border.color: Ui.style.pipelineLineColor
        border.width: Ui.style.pipelineLineWidth
    }
}
