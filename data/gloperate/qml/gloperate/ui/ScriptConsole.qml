
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  ScriptConsole
*
*  Scripting console
*/
Item
{
    id: panel

    Rectangle
    {
        anchors.top:          parent.top
        anchors.bottom:       input.top
        anchors.bottomMargin: Ui.style.panelSpacing
        width:                parent.width

        color: Ui.style.panelColorAlt

        ScrollArea
        {
            anchors.fill: parent

            contentHeight: content.height

            Column
            {
                id: content

                anchors.left:    parent.left
                anchors.right:   parent.right
                anchors.top:     parent.top
                anchors.margins: Ui.style.ctrlPadding

                Repeater
                {
                    width: parent.width

                    model: 200

                    Label
                    {
                        id: text

                        width: content.width
                        text:  "Command"
                    }
                }
            }
        }
    }

    TextField
    {
        id: input

        anchors.bottom:  parent.bottom
        width:           parent.width
        height:          24
    }
}
