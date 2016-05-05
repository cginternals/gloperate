
import QtQuick 2.0
import gloperate.base 1.0


/**
*  PanelItem
*
*  An item on a panel that can be selected
*/
Control
{
    id: item

    // Button icon
    property string icon: ''

    // Button text
    property string text: ''

    // Actual width of the item
    readonly property real buttonWidth: row.implicitWidth  + 2 * Ui.style.panelPadding

    implicitWidth:  row.implicitWidth  + 2 * Ui.style.panelPadding
    implicitHeight: row.implicitHeight + 2 * Ui.style.panelPadding

    Rectangle
    {
        id: background

        anchors.fill: parent
        color:        Ui.style.getPanelItemColor(item.enabled, item.highlighted, item.hovered, item.pressed)
    }

    Row
    {
        id: row

        anchors.fill:    parent
        anchors.margins: Ui.style.panelPadding
        spacing:         Ui.style.panelSpacing

        Icon
        {
            anchors.verticalCenter: parent.verticalCenter

            visible: item.icon != ''
            icon:    item.icon
            color:   Ui.style.getControlIconColor(item.enabled, item.highlighted, item.hovered, item.pressed)
            enabled: false
        }

        Label
        {
            anchors.verticalCenter: parent.verticalCenter

            text:    item.text
            color:   Ui.style.getControlTextColor(item.enabled, item.highlighted, item.hovered, item.pressed)
            enabled: false
        }
    }
}
