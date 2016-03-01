
pragma Singleton


import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Ui
*
*  Global options for the UI
*/
Item
{
    // Style
    property Item style: styleLight

    // Available styles
    readonly property var styles: [
        'Light', 'Dark'
    ]

    // Debug mode
    property bool debugMode: false

    // Set style
    function setStyle(name)
    {
        if (name == 'Light') style = styleLight;
        else                 style = styleDark;
    }

    // Styles
    Style
    {
        id: styleLight
    }

    StyleDark
    {
        id: styleDark
    }
}
