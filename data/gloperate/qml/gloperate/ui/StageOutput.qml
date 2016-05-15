
import QtQuick 2.0
import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  StageOutput
*
*  Representation of an output
*/
BaseItem
{
    id: item

    property string name:       'Output'
    property string value:      ''
    property real   circleSize: Ui.style.formControlHeight
    property color  lineColor:  Ui.style.pipelineLineColor
    property color  fillColor:  'green'

    implicitWidth:  label.implicitWidth + 2 * label.anchors.margins + circleSize
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
            var circleX = width - circleSize - 2;
            var circleY = centerY - circleSize / 2.0;

            ctx.ellipse(circleX, circleY, circleSize, circleSize);
            ctx.fill();
            ctx.stroke();

            ctx.moveTo(0, centerY);
            ctx.lineTo(circleX, centerY);
            ctx.stroke();

            ctx.restore();
        }
    }

    Label
    {
        id: label

        anchors.left:    parent.left
        anchors.top:     parent.verticalCenter
        anchors.margins: Ui.style.ctrlPadding

        text: item.name
    }

    Label
    {
        id: labelValue

        anchors.left:    parent.left
        anchors.bottom:  parent.verticalCenter
        anchors.margins: Ui.style.ctrlPadding

        text: item.value
    }
}
