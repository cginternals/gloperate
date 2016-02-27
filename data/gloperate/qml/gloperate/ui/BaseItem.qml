
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  BaseItem
*
*  Base class for items (adds debug capabilities)
*/
Rectangle
{
    property bool debug: Ui.debug

    color:       'transparent'
    border.color: debug ? 'red' : 'transparent'
    border.width: debug ? 1 : 0
}
