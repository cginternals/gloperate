
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

    // Internals
    property bool loaded: false

    implicitWidth:  scrollArea.contentWidth  + 20
    implicitHeight: scrollArea.contentHeight + 20

    // Interface for communicating with the actual pipeline
    PipelineInterface
    {
        id: pipelineInterface
    }

    // Scrollable container
    ScrollArea
    {
        id: scrollArea

        anchors.fill:  parent

        contentWidth:  pipeline.width
        contentHeight: pipeline.height + menuBar.height

        // Main menu
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

        // Pipeline visualization
        Pipeline
        {
            id: pipeline

            anchors.top:  menuBar.bottom
            anchors.left: parent.left

            pipelineInterface: pipelineInterface
        }
    }

    /**
    *  Load pipeline
    *
    *  @param[in] root
    *    Pipeline object
    */
    function load(root)
    {
        // Check if pipeline has already been loaded
        if (!loaded)
        {
            // Set pipeline root
            pipelineInterface.root = root;

            // Get name of pipeline
            var name = pipelineInterface.getStage('pipeline').stages[0];
            pipeline.path = 'pipeline.' + name;

            // Done
            loaded = true;
        }
    }
}
