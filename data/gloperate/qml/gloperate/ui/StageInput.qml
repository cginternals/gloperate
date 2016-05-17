
import QtQuick 2.0
import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  StageInput
*
*  Representation of an input slot
*/
BaseItem
{
    id: item

    property string name:       'Input'
    property string value:      ''
    property real   circleSize: Ui.style.formControlHeight
    property color  lineColor:  Ui.style.pipelineLineColor
    property color  fillColor:  'green'

    implicitWidth:  label.implicitWidth + 2 * label.anchors.margins + circleSize * 0.5
    implicitHeight: 2 * (label.implicitHeight + label.anchors.margins)

    onLineColorChanged:  canvas.requestPaint();
    onFillColorChanged:  canvas.requestPaint();
    onCircleSizeChanged: canvas.requestPaint();

    Canvas
    {
        id: canvas

        anchors.fill: parent

        onPaint:
        {
            var ctx = canvas.getContext('2d');

            ctx.fillStyle = item.fillColor;
            ctx.strokeStyle = item.lineColor;

            var centerY = height / 2.0;
            var circleX = circleSize / 2.0;

            ctx.arc(0, centerY, circleSize / 2.0, Math.PI * 0.5, Math.PI * 1.5, true);
            ctx.stroke();

            ctx.moveTo(circleX, centerY);
            ctx.lineTo(width,   centerY);
            ctx.stroke();

            ctx.restore();
        }
    }

    Label
    {
        id: label

        anchors.right:   parent.right
        anchors.top:     parent.verticalCenter
        anchors.margins: Ui.style.ctrlPadding

        text: item.name
    }

    Label
    {
        id: labelValue

        anchors.right:   parent.right
        anchors.bottom:  parent.verticalCenter
        anchors.margins: Ui.style.ctrlPadding

        text: item.value
    }
}
