
pragma Singleton


import QtQuick 2.0


/**
*  Style
*
*  Defines the visual style for the application (e.g., colors, and sizes)
*/
QtObject
{
    //
    // General styles (to be used by the specialized options)
    //

    // General colors
    property color backgroundColor: '#ffffff'
    property color borderColor:     '#000000'
    property color textColor:       '#000000'

    // Fonts
    property int  fontSizeHuge:     32
    property int  fontSizeLarge:    20
    property int  fontSizeMedium:   16
    property int  fontSizeSmall:    14
    property int  fontSizeTiny:     12

    // Padding sizes
    property real paddingLarge:     12
    property real paddingMedium:    6
    property real paddingSmall:     3


    //
    // Item specific options
    //

    // Label
    property color labelColor:     textColor
    property int   labelFontSize:  fontSizeMedium
}
