
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

    property var log: []

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

                    model: panel.log.length

                    Label
                    {
                        id: text

                        width: content.width
                        text:  panel.log[index]
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
        height:          Ui.style.formControlHeight

        onAccepted:
        {
            var log = panel.log;
            log.push(eval(text));
            panel.log = log;

            text = '';
        }
    }
}
