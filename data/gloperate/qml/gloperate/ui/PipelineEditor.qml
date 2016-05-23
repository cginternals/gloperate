
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

        Row
        {
            id: topBar

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
                    pipeline.update();
                }
            }
        }

        Stage
        {
            id: pipeline

            anchors.left:    parent.left
            anchors.top:     parent.top
            anchors.margins: Ui.style.pipelinePadding
            stageDelegate:   stageComponent
            targetStage:     'Viewer'
        }
    }

    Component.onCompleted:
    {
        /*
        gloperate.pipeline.registerWatcher(function()
        {
            // [TODO]
            // Update only the item that has been changed
        });
        */
    }

    // [TODO] After a while, this crashes (the faster, the sooner)
    //        Check if this happens also with qml-only operations,
    //        or if something in e.g. the qml-cppexpose-bridge
    //        causes a memory leak
    /*
    Timer
    {
        interval: 200
        running:  true
        repeat:   true

        onTriggered:
        {
            pipeline.update();
        }
    }
    */
}
