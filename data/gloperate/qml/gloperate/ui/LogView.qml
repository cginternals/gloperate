
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  LogViewer
*
*  Displays a list of log messages
*/
ScrollArea
{
    contentHeight: content.height

    Rectangle
    {
        anchors.fill: parent

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
            model: 200
            width: parent.width
            height: 32

            Label
            {
                id: text

                width: content.width
                text:  "Log Message"
            }
        }
    }
}
