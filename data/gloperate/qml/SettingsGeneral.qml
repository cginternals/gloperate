
import QtQuick 2.0
import QtQuick.Controls 1.1

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


/**
*  SettingsGeneral
*
*  Settings page 'General'
*/
Item
{
    property real topMargin: 0

    ScrollArea
    {
        anchors.fill:      parent
        anchors.topMargin: topMargin

        contentHeight: col.height

        Column
        {
            id: col

            anchors.top:     parent.top
            anchors.left:    parent.left
            anchors.right:   parent.right
            anchors.margins: Ui.style.paddingMedium

            spacing: Ui.style.spacingMedium

            GroupBox
            {
                title: "Theme"
                width: parent.width

                ExclusiveGroup
                {
                    id: themeGroup
                }

                Column
                {
                    spacing: Ui.style.spacingMedium

                    Repeater
                    {
                        model: Ui.styles.length

                        RadioButton
                        {
                            exclusiveGroup: themeGroup
                            text:           Ui.styles[index]
                            checked:        Ui.styleName == Ui.styles[index]

                            onClicked:
                            {
                                if (checked) {
                                    Ui.setStyle(text);
                                }
                            }
                        }
                    }
                }
            }

            GroupBox
            {
                title: "Debug"
                width: parent.width

                Column
                {
                    width:   parent.width
                    spacing: Ui.style.spacingMedium

                    Switch
                    {
                        width:   parent.width
                        checked: false
                        text:    'Enable debug mode'

                        onClicked:
                        {
                            Ui.debugMode = checked;
                        }
                    }
                }
            }
        }
    }
}
