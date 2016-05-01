
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Settings
*
*  Settings dialog
*/
Background
{
    id: panel

    property var pages: [
        { text: 'General', icon: '0147-equalizer.png' },
        { text: 'Input',   icon: '0086-keyboard.png' },
        { text: 'Plugins', icon: '0169-hammer2.png' }
    ]

    property int selected: 0

    // Left pane
    Rectangle
    {
        id: leftPane

        anchors.left:   parent.left
        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        width:          250

        color: Ui.style.panelColorAlt

        ScrollArea
        {
            anchors.fill: parent

            contentHeight: leftCol.height

            Column
            {
                id: leftCol

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

    // Page 'General'
    SettingsGeneral
    {
        id: pageGeneral

        anchors.left:   leftPane.right
        anchors.right:  parent.right
        anchors.top:    parent.top
        anchors.bottom: parent.bottom

        visible: panel.pages[panel.selected].text == 'General'
    }

    // Page 'Input'
    SettingsInput
    {
        id: pageInput

        anchors.left:   leftPane.right
        anchors.right:  parent.right
        anchors.top:    parent.top
        anchors.bottom: parent.bottom

        visible: panel.pages[panel.selected].text == 'Input'
    }

    // Page 'Plugins'
    SettingsPlugins
    {
        id: pagePlugins

        anchors.left:   leftPane.right
        anchors.right:  parent.right
        anchors.top:    parent.top
        anchors.bottom: parent.bottom

        visible: panel.pages[panel.selected].text == 'Plugins'
    }
}
