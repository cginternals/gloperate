
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
    // Debug mode
    property bool debugMode: false

    // Available styles
    readonly property var styles: [
        'Light', 'Dark'
    ]

    // Style selection
    property string styleName: 'Light';

    function setStyle(name)
    {
        styleName = name;
    }

    // Style
    readonly property Item style: getStyle(styleName)

    function getStyle(name)
    {
        if (name == 'Light') return styleLight;
        else                 return styleDark;
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
