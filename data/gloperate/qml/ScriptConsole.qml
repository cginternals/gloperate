
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

    property string log: 'Welcome to scripting.\n'

    Rectangle
    {
        anchors.fill: parent
        color:        Ui.style.panelColorAlt
    }

    ScrollArea
    {
        anchors.fill:  parent
        contentHeight: content.height + 2 * Ui.style.ctrlPadding

        Column
        {
            id: content

            anchors.left:    parent.left
            anchors.right:   parent.right
            anchors.top:     parent.top
            anchors.margins: Ui.style.ctrlPadding
            spacing:         Ui.style.ctrlPadding

            TextField
            {
                id: output

                width:           parent.width
                backgroundColor: Ui.style.panelColorAlt
                text:            panel.log
                readOnly:        true
            }

            TextField
            {
                id: input

                width:           parent.width
                backgroundColor: Ui.style.ctrlBorderColorPressed

                onAccepted:
                {
                    var log = panel.log;
                    log += '\n' + text;
//                  log += eval(text) + '\n';
                    panel.log = log;

                    text = '';
                }
            }
        }
    }
}
