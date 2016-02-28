
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Control
*
*  Base class for controls
*/
BaseItem
{
    id: item

    // Is the item highlighted?
    property bool highlighted: false

    // Is the mouse currently over the item?
    property alias hovered: mouseArea.containsMouse

    // Is a mouse button currently pressed over the item?
    property alias pressed: mouseArea.pressed

    // Called when a mouse button has been clicked on the item
    signal clicked()

    MouseArea
    {
        id: mouseArea

        enabled:      item.enabled
        anchors.fill: parent
        hoverEnabled: true

        onClicked:
        {
            item.clicked();
        }
    }
}
