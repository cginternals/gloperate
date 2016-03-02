
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Panel
*
*  Container on a page.
*  Automatically adds scroll bars if its content is
*  larger than the panel itself.
*/
BaseItem
{
    default property alias data: content.data

    Rectangle
    {
        id: rect

        anchors.fill: parent

        color:        Ui.style.panelColor
        border.color: Ui.style.panelBorderColor
        border.width: Ui.style.panelBorderWidth
        radius:       Ui.style.panelBorderRadius

        Item
        {
            id: content

            anchors.fill:    parent
            anchors.margins: rect.border.width
        }
    }
}
