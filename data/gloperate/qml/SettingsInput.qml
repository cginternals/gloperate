
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


/**
*  SettingsInput
*
*  Settings page 'Input'
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
        }
    }
}
