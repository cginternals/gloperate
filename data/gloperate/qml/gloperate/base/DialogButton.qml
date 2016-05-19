
import QtQuick 2.0
import gloperate.base 1.0


/**
*  DialogButton
*
*  A push button with a label and an optional icon, used in e.g. dialogs.
*/
Control
{
    id: item

    // Button icon
    property string icon: ''

    // Button text
    property string text: ''

    // Actual width of the button
    readonly property real buttonWidth: row.implicitWidth  + 2 * Ui.style.ctrlPadding

    implicitWidth:  row.implicitWidth  + 2 * Ui.style.ctrlPadding
    implicitHeight: row.implicitHeight + 2 * Ui.style.ctrlPadding

    Rectangle
    {
        id: background

        anchors.fill: parent
        color:        Ui.style.getControlColor(item.enabled, item.highlighted, item.hovered, item.pressed)
        border.color: Ui.style.getControlTextColor(item.enabled, item.highlighted, item.hovered, item.pressed)
        border.width: Ui.style.borderWidth
        radius:       Ui.style.ctrlRadius
    }

    Row
    {
        id: row

        anchors.fill:    parent
        anchors.margins: Ui.style.ctrlPadding
        spacing:         Ui.style.ctrlSpacing

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
