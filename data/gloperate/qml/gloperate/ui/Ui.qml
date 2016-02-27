
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
    property Item style: styleDefault

    // Debug mode
    property bool debug: false


    // Styles
    Style
    {
        id: styleDefault
    }

    StyleDark
    {
        id: styleDark
    }
}
