
import QtQuick 2.0
import gloperate.base 1.0


/**
*  BaseItem
*
*  Base class for items (adds debug capabilities)
*/
Item
{
    id: item

    // Debug mode enabled? (displays borders around items)
    property bool debugMode: Ui.debugMode

    clip: true

    Rectangle
    {
        id: debugBorder

        anchors.fill: parent
        z:            10
        color:        'transparent'
        border.color: debugMode ? Ui.style.debugColor : 'transparent'
        border.width: debugMode ? 1 : 0
    }
}
