
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

    ScrollArea
    {
        id: scrollArea

        anchors.fill:  parent
        contentWidth:  pipeline.width
        contentHeight: pipeline.height

        Stage
        {
            id: pipeline

            targetStage: 'Root'
        }
    }
}
