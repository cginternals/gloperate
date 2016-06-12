
pragma Singleton


import QtQuick 2.0
import gloperate.base 1.0


/**
*  Ui
*
*  Global options for the UI
*/
Item
{
    id: item

    // Debug mode
    property bool debugMode: false

    // Available styles
    readonly property var styles: [
        'Light', 'Dark'
    ]

    // Style selection
    property string styleName: 'Light';

    readonly property Item style: getStyle(styleName)

    function getStyle(name)
    {
        if (name == 'Light') return styleLight;
        else                 return styleDark;
    }

    function setStyle(name)
    {
        styleName = name;

        // Update config
        if (typeof(config) !== 'undefined')
        {
            config.style = name;
        }
    }

    // Styles
    StyleLight
    {
        id: styleLight
    }

    StyleDark
    {
        id: styleDark
    }

    // Initialize from config
    Component.onCompleted:
    {
        if (typeof(config) !== 'undefined')
        {
            if (config.style)
            {
                item.styleName = config.style;
            }
        }
    }
}
