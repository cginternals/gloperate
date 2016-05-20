
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
        contentWidth:  pipeline.width  + 2 * pipeline.anchors.margins
        contentHeight: pipeline.height + 2 * pipeline.anchors.margins

        Component
        {
            id: stageComponent

            Stage
            {
                stageDelegate: stageComponent
                targetStage:   modelData
            }
        }

        Stage
        {
            id: pipeline

            anchors.left:    parent.left
            anchors.top:     parent.top
            anchors.margins: Ui.style.pipelinePadding
            stageDelegate:   stageComponent
            targetStage:     'Root'
        }
    }
}
