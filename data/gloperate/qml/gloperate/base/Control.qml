
import QtQuick 2.0
import gloperate.base 1.0


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

    // Called when the left mouse button has been clicked on the item
    signal clicked()

    // Called when the right mouse button has been clicked on the item
    signal rightClicked()

    MouseArea
    {
        id: mouseArea

        enabled:      item.enabled
        anchors.fill: parent
        hoverEnabled: true

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked:
        {
            if (mouse.button == Qt.RightButton)
                item.rightClicked();
            else
                item.clicked();
        }
    }
}
