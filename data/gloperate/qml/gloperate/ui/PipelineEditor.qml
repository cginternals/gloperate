
import QtQuick 2.0
import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  PipelineEditor
*
*  Dialog for editing a pipeline
*/
Background
{
    id: panel

    implicitWidth:  scrollArea.contentWidth  + 20
    implicitHeight: scrollArea.contentHeight + 20

    property bool loaded: false

    ScrollArea
    {
        id: scrollArea

        anchors.fill:  parent

        contentWidth:  pipeline.width
        contentHeight: pipeline.height + menuBar.height

        Row
        {
            id: menuBar

            anchors.top:     parent.top
            anchors.left:    parent.left
            anchors.margins: Ui.style.pagePadding
            spacing:         Ui.style.pageSpacing

            Button
            {
                icon: '0270-cancel-circle.png'
                text: 'Close'

                onClicked:
                {
                    panel.visible = false;
                }
            }
        }

        Pipeline
        {
            id: pipeline

            anchors.top:  menuBar.bottom
            anchors.left: parent.left
        }
    }

    function load()
    {
        // Check if pipeline has already been loaded
        if (!loaded)
        {
            // Get pipeline container
            var pipelineContainer = gloperate.canvas0.pipeline;

            // Load root pipeline
            var pipelineName = pipelineContainer.getDescription().stages[0];
            pipeline.path = pipelineName;

            // Done
            loaded = true;
        }
    }
}
