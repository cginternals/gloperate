
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Panel
*
*  Container on a page
*/
BaseItem
{
    Rectangle
    {
        anchors.fill: parent

        color:        Ui.style.panelColor
        border.color: Ui.style.panelBorderColor
        border.width: Ui.style.panelBorderWidth
        radius:       Ui.style.panelRadius
    }
}
