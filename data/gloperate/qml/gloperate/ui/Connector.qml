
import QtQuick 2.0

import gloperate.base 1.0


/**
*  Connector
*
*  Connection between two slots in a pipeline
*/
Item
{
    property int x0:        0
    property int y0:        0
    property int x1:        0
    property int y1:        0
    property int arrowSize: Ui.style.pipelineConnectorSize / 3.0

    Canvas
    {
        id: canvas

        anchors.fill: parent

        height: parent.height / 2
        width: parent.width

        antialiasing: true

        onPaint:
        {
            var ctx = canvas.getContext('2d');

            ctx.clearRect(0, 0, width, height);

            ctx.strokeStyle = '#000000';
            ctx.lineWidth = 2;

            ctx.beginPath();
            ctx.moveTo(x0, y0);
            ctx.bezierCurveTo(x0, y0, x0 + 100, y0, (x0 + x1) / 2.0, (y0 + y1) / 2.0);
            ctx.bezierCurveTo((x0 + x1) / 2.0, (y0 + y1) / 2.0, x1 - 100, y1, x1, y1);
            ctx.stroke();

            var l = arrowSize;

            ctx.beginPath();
            ctx.moveTo(x1, y1);
            ctx.lineTo(x1 - l, y1 - l);
            ctx.lineTo(x1 - l, y1 + l);
            ctx.lineTo(x1, y1);
            ctx.fill();
        }
    }

    onX0Changed: canvas.requestPaint();
    onY0Changed: canvas.requestPaint();
    onX1Changed: canvas.requestPaint();
    onY1Changed: canvas.requestPaint();
}
