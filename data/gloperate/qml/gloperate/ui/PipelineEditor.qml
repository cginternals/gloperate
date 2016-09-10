
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
        contentWidth:  1000
        contentHeight: 1000

        Connector
        {
            anchors.fill: parent

            x0: stage1.getOutputPos(0, stage1.x, stage1.y).x
            y0: stage1.getOutputPos(0, stage1.x, stage1.y).y
            x1: stage2.getInputPos (0, stage2.x, stage2.y).x
            y1: stage2.getInputPos (0, stage2.x, stage2.y).y
        }

        Connector
        {
            anchors.fill: parent

            x0: stage1.getOutputPos(1, stage1.x, stage1.y).x
            y0: stage1.getOutputPos(1, stage1.x, stage1.y).y
            x1: stage2.getInputPos (3, stage2.x, stage2.y).x
            y1: stage2.getInputPos (3, stage2.x, stage2.y).y
        }

        Connector
        {
            anchors.fill: parent

            x0: stage1.getOutputPos(2, stage1.x, stage1.y).x
            y0: stage1.getOutputPos(2, stage1.x, stage1.y).y
            x1: stage2.getInputPos (1, stage2.x, stage2.y).x
            y1: stage2.getInputPos (1, stage2.x, stage2.y).y
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
                    // pipeline.update();
                }
            }
        }

        Stage2
        {
            id: stage1

            x:  50
            y: 100
        }

        Stage2
        {
            id: stage2

            x: 500
            y: 100
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
