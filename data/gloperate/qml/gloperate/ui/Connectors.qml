
import QtQuick 2.0

import gloperate.base 1.0


/**
*  Connectors
*
*  Draws the connections between stages
*/
Item
{
    property Item stages:    null
    property Item pipeline:  null
    property int  arrowSize: Ui.style.pipelineConnectorSize / 3.0

    Canvas
    {
        id: canvas

        anchors.fill: parent

        antialiasing: true

        onPaint:
        {
            var ctx = canvas.getContext('2d');

            ctx.clearRect(0, 0, width, height);

            drawConnectors(ctx);
        }

        function drawConnectors(ctx)
        {
            // Get pipeline
            var pl     = pipeline.getStage(pipeline.path);
            var plDesc = pl.getDescription();

            // Get all stages of the pipeline and the pipeline itself
            var stages = [];

            for (var i in plDesc.stages)
            {
                var st = pl[ plDesc.stages[i] ];
                stages.push(st);
            }

            stages.push(pl);

            // Get connectors
            for (var i in stages)
            {
                // Get stage
                var st     = stages[i];
                var stDesc = st.getDescription();

                // Get connections of the stage
                var connections = st.getConnections();
                for (var j in connections)
                {
                    // Get connection
                    var connection = connections[j];
                    var from = connection.from;
                    var to   = connection.to;

                    // Draw connection
                    var p0 = pipeline.getSlotPos(from, 'output');
                    var p1 = pipeline.getSlotPos(to,   'input');

                    if (p0 != null && p1 != null)
                    {
                        // Highlight the connection if its input or output slot is selected
                        var status = (pipeline.hoveredElement == from || pipeline.hoveredElement == to) ? 1 : 0;

                        drawConnector(ctx, p0, p1, status);
                    }
                }
            }

            // Draw interactive connector
            if (pipeline.selectedOutput != '')
            {
                var p0 = pipeline.getSlotPos(pipeline.selectedOutput, 'output');
                var p1 = { x: pipeline.mouseX, y: pipeline.mouseY };
                drawConnector(ctx, p0, p1, 2);
            }

            if (pipeline.selectedInput != '')
            {
                var p0 = { x: pipeline.mouseX, y: pipeline.mouseY };
                var p1 = pipeline.getSlotPos(pipeline.selectedInput, 'input');
                drawConnector(ctx, p0, p1, 2);
            }
        }

        function drawConnector(ctx, p0, p1, status)
        {
            var x0 = p0.x;
            var y0 = p0.y;
            var x1 = p1.x;
            var y1 = p1.y;

            var color = Ui.style.pipelineColorDefault;
            if (status == 1) color = Ui.style.pipelineColorHighlighted;
            if (status == 2) color = Ui.style.pipelineColorSelected;

            ctx.strokeStyle = color;
            ctx.fillStyle   = color;
            ctx.lineWidth   = 2;

            ctx.beginPath();
            ctx.moveTo(x0, y0);
            ctx.bezierCurveTo(x0, y0, x0 + 100, y0, (x0 + x1) / 2.0, (y0 + y1) / 2.0);
            ctx.bezierCurveTo((x0 + x1) / 2.0, (y0 + y1) / 2.0, x1 - 100, y1, x1, y1);
            ctx.stroke();

            ctx.beginPath();
            ctx.moveTo(x1, y1);
            ctx.lineTo(x1 - arrowSize, y1 - arrowSize);
            ctx.lineTo(x1 - arrowSize, y1 + arrowSize);
            ctx.lineTo(x1, y1);
            ctx.fill();
        }
    }

    function requestPaint()
    {
        canvas.requestPaint();
    }
}
