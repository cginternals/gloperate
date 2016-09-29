
import QtQuick 2.0

import gloperate.base 1.0


/**
*  Connectors
*
*  Draws the connections between stages
*/
Item
{
    id: connectors

    // Options
    property var    pipelineInterface: null ///< Interface for accessing the pipeline
    property Item   pipeline:          null ///< Pointer to pipeline item
    property string path:              ''   ///< Path in the pipeline hierarchy (e.g., 'pipeline')
    property int    arrowSize:         Ui.style.pipelineConnectorSize / 3.0

    // 2D canvas for drawing
    Canvas
    {
        id: canvas

        anchors.fill: parent

        antialiasing: true

        /**
        *  Draw on canvas
        */
        onPaint:
        {
            // Get draw context
            var ctx = canvas.getContext('2d');

            // Clear canvas
            ctx.clearRect(0, 0, width, height);

            // Draw connectors
            drawConnectors(ctx);
        }

        /**
        *  Draw all connectors of the given pipeline
        *
        *  @param[in] ctx
        *    Draw context
        */
        function drawConnectors(ctx)
        {
            // Get all stages of the pipeline and the pipeline itself
            var stages = [];

            stages.push(path);

            var pipeline = pipelineInterface.getStage(path);

            for (var i in pipeline.stages)
            {
                stages.push(path + '.' + pipeline.stages[i]);
            }

            // Get connectors
            for (var i in stages)
            {
                // Get stage
                var stage = stages[i];

                // Get connections of the stage
                var connections = pipelineInterface.getConnections(stage);
                for (var j in connections)
                {
                    // Get connection
                    var connection = connections[j];
                    var from = connection.from;
                    var to   = connection.to;

                    // Draw connection
                    var p0 = connectors.pipeline.getSlotPos(from);
                    var p1 = connectors.pipeline.getSlotPos(to);

                    if (p0 != null && p1 != null)
                    {
                        // Highlight the connection if its input or output slot is selected
                        var status = (connectors.pipeline.hoveredElement == from || connectors.pipeline.hoveredElement == to) ? 1 : 0;

                        drawConnector(ctx, p0, p1, status);
                    }
                }
            }

            // Draw interactive connector
            if (connectors.pipeline.selectedOutput != '')
            {
                var p0 = connectors.pipeline.getSlotPos(connectors.pipeline.selectedOutput);
                var p1 = { x: connectors.pipeline.mouseX, y: connectors.pipeline.mouseY };
                drawConnector(ctx, p0, p1, 2);
            }

            if (connectors.pipeline.selectedInput != '')
            {
                var p0 = { x: connectors.pipeline.mouseX, y: connectors.pipeline.mouseY };
                var p1 = connectors.pipeline.getSlotPos(connectors.pipeline.selectedInput);
                drawConnector(ctx, p0, p1, 2);
            }
        }

        /**
        *  Draw a connector
        *
        *  @param[in] ctx
        *    Draw context
        *  @param[in] p0
        *    Start position (x, y)
        *  @param[in] p1
        *    End position (x, y)
        *  @param[in] status
        *    Connector state (0: normal, 1: highlighted, 2: selected)
        */
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

    /**
    *  Request redraw
    */
    function requestPaint()
    {
        canvas.requestPaint();
    }
}
