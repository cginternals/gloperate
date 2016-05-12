
import QtQuick 2.0
import QtQuick.Layouts 1.0
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
        contentWidth:  row.width  + 2 * row.anchors.margins
        contentHeight: row.height + 2 * row.anchors.margins

        Row
        {
            id: row

            anchors.left:    parent.left
            anchors.top:     parent.top
            anchors.margins: 16

            spacing: 32

            Repeater
            {
                model: 1

                delegate: ColumnLayout
                {
                    spacing: 32

                    Stage
                    {
                        Layout.fillWidth: true

                        name:        gloperate.pipeline.getName()
                        inputNames:  gloperate.pipeline.getInputs()
                        outputNames: gloperate.pipeline.getOutputs()
                    }
                }
            }
        }
    }
}
