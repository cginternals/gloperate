
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
                icon: '0133-spinner11.png'
                text: 'Update'

                onClicked:
                {
                    // pipeline.update();
                }
            }

            Button
            {
                icon: '0267-plus.png'
                text: 'Add Stage'

                onClicked:
                {
                    pipeline.addStage();
                }
            }

            Button
            {
                icon: '0270-cancel-circle.png'
                text: 'Clear stages'

                onClicked:
                {
                    pipeline.clear();
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

    Component.onCompleted:
    {
        // Get pipeline container
        var pipelineContainer = gloperate.canvas0.pipeline;

        // Load root pipeline
        var pipelineName = pipelineContainer.getDescription().stages[0];
        pipeline.load(pipelineName);
    }
}
