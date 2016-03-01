
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  ConfigDialog
*
*  Configuration dialog
*/
DockPanel
{
    id: panel

    iconClosed: '0270-cancel-circle.png'
    iconOpen:   '0270-cancel-circle.png'

    property var pages: [
        { text: 'General', icon: '0147-equalizer.png' },
        { text: 'Input',   icon: '0086-keyboard.png' },
        { text: 'Plugins', icon: '0169-hammer2.png' }
    ]

    property int selected: 0

    Rectangle
    {
        id: left

        anchors.left:   parent.left
        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        width:          250

        color: Ui.style.panelColorAlt

        ScrollArea
        {
            anchors.fill: parent

            contentHeight: col.height

            Column
            {
                id: col

                anchors.left:  parent.left
                anchors.right: parent.right

                Repeater
                {
                    model: panel.pages.length

                    PanelItem
                    {
                        width:  parent.width
                        height: 40

                        text: panel.pages[index].text
                        icon: panel.pages[index].icon

                        highlighted: index == panel.selected

                        onClicked:
                        {
                            panel.selected = index;
                        }
                    }
                }
            }
        }
    }
}
