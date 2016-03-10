
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  LogViewer
*
*  Displays a list of log messages
*/
ScrollArea
{
    id: panel

    property var log: []

    contentHeight: content.height + 2 * Ui.style.ctrlPadding

    Rectangle
    {
        width:  panel.width
        height: Math.max(panel.height, parent.height)

        color: Ui.style.panelColorAlt
    }

    Column
    {
        id: content

        anchors.left:    parent.left
        anchors.right:   parent.right
        anchors.top:     parent.top
        anchors.margins: Ui.style.ctrlPadding

        Repeater
        {
            model: panel.log.length

            Label
            {
                id: text

                width: parent.width
                text:  panel.log[index]
            }
        }
    }
}
