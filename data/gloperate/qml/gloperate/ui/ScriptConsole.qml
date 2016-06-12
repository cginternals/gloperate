
import QtQuick 2.0
import gloperate.base 1.0


/**
*  ScriptConsole
*
*  Scripting console
*/
Item
{
    id: panel

    property string log: 'Welcome to scripting.\n'

    ScrollArea
    {
        id: scrollArea

        anchors.fill:  parent
        contentHeight: content.height + 2 * Ui.style.ctrlPadding

        Column
        {
            id: content

            anchors.left:  parent.left
            anchors.right: parent.right
            anchors.top:   parent.top
            spacing:       Ui.style.ctrlPadding

            LineEdit
            {
                id: output

                width:    parent.width
                text:     panel.log
                readOnly: true
            }

            LineEdit
            {
                id: input

                width:           parent.width
                backgroundColor: Ui.style.panelColorAlt

                onAccepted:
                {
                    var cmd = text;
                    text = '';

                    var log = panel.log;

                    log += '\n> ' + cmd;
                    panel.log = log;

                    log += '\n' + execute(cmd) + '\n';
                    panel.log = log;

                    scrollArea.contentY = scrollArea.contentHeight;
                }
            }
        }
    }
}
