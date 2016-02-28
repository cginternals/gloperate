
import QtQuick 2.0


/**
*  Style
*
*  Defines the visual style for the application (e.g., colors, and sizes)
*/
Item
{
    //
    // Abstract style definition (to be used by the generic options)
    //

    // Colors
    property color primaryColor:    '#000000'
    property color secondaryColor:  '#cccccc'

    property color disabledColor:   '#999999'

    property color backgroundColor: '#44aaaaaa'
    property color windowColor:     '#44aaaaaa'

    property color controlColor:    '#ffffff'
    property color controlColorHov: '#f0f0f0'
    property color controlColorHl:  '#d0d0d0'
    property color controlTextColor:'#666666'

    property color highlightColor:  '#4078c0'
    property color activeColor:     '#6098e0'

    property color borderColor:     '#888888'
    property int   borderWidth:     1

    property real  borderRadius:    3

    // Fonts
//  property int   fontSizeHuge:    32
//  property int   fontSizeLarge:   20
    property int   fontSizeMedium:  16
    property int   fontSizeSmall:   14
//  property int   fontSizeTiny:    12

    // Padding sizes
//  property real  paddingLarge:    12
    property real  paddingMedium:   4
    property real  paddingSmall:    2


    //
    // Generic style definition (to be used by specific items)
    //

    // Main page
    property color pageColor:                  '#ffffff'      // x Page background
    property real  pagePadding:                paddingMedium  // x Page padding
    property real  pageSpacing:                paddingMedium  // Page spacing

    property color pageTextColor:              primaryColor   // Page text color (default)
    property color pageTextColorSecondary:     secondaryColor // Page text color (secondary texts)
    property color pageTextColorTitle:         primaryColor   // Page text color (title texts)
    property color pageTextColorDisabled:      '#000000'      // Page text color (disabled texts)
    property color pageTextColorEmphasized:    '#000000'      // Page text color (emphasized texts)

    property int   pageTextSize:               fontSizeMedium // Page text size (default)
    property int   pageTextSizeSecondary:      fontSizeMedium // Page text size (secondary texts)
    property int   pageTextSizeTitle:          fontSizeMedium // Page text size (title texts)
    property int   pageTextSizeDisabled:       fontSizeMedium // Page text size (disabled texts)
    property int   pageTextSizeEmphasized:     fontSizeMedium // Page text size (emphasized texts)

    property color pageLinkColor:              primaryColor   // Page link color (default)
    property color pageLinkColorDisabled:      primaryColor   // Page link color (disabled)
    property color pageLinkColorHover:         primaryColor   // Page link color (hovered)
    property color pageLinkColorPressed:       primaryColor   // Page link color (pressed)

    property color pageIconColor:              primaryColor   // Page icon color
    property int   pageIconSize:               64             // Page icon size

    // Panel
    property color panelColor:                 windowColor    // x Panel background
    property color panelBorderColor:           borderColor    // x Panel border color
    property int   panelBorderWidth:           borderWidth    // x Panel border width
    property real  panelPadding:               paddingMedium  // x Panel padding
    property real  panelSpacing:               paddingMedium  // Panel spacing
    property real  panelRadius:                borderRadius   // x Panel radius (round edges)

    property color panelTextColor:             primaryColor   // Panel text color (default)
    property color panelTextColorSecondary:    secondaryColor // Panel text color (secondary texts)
    property color panelTextColorTitle:        primaryColor   // Panel text color (title texts)
    property color panelTextColorDisabled:     '#000000'      // Panel text color (disabled texts)
    property color panelTextColorEmphasized:   '#000000'      // Panel text color (emphasized texts)

    property int   panelTextSize:              fontSizeMedium // Panel text size (default)
    property int   panelTextSecondary:         fontSizeMedium // Panel text size (secondary texts)
    property int   panelTextTitle:             fontSizeMedium // Panel text size (title texts)
    property int   panelTextDisabled:          fontSizeMedium // Panel text size (disabled texts)
    property int   panelTextEmphasized:        fontSizeMedium // Panel text size (emphasized texts)

    property color panelLinkColor:             primaryColor   // Panel link color (default)
    property color panelLinkColorDisabled:     primaryColor   // Panel link color (disabled)
    property color panelLinkColorHover:        primaryColor   // Panel link color (hovered)
    property color panelLinkColorPressed:      primaryColor   // Panel link color (pressed)

    property color panelIconColor:             controlTextColor // Panel icon color
    property int   panelIconSize:              64               // Panel icon size

    // Controls
    property real  ctrlPadding:                paddingMedium  // x Control padding
    property real  ctrlSpacing:                paddingMedium  // Control padding
    property real  ctrlRadius:                 borderRadius   // x Control radius (round edges)

    property color ctrlColor:                  controlColor   // x Control background color (default)
    property color ctrlColorDisabled:          controlColor   // Control background color (disabled)
    property color ctrlColorHighlighted:       controlColorHl // Control background color (highlighted)
    property color ctrlColorHover:             controlColorHov// Control background color (hovered)
    property color ctrlColorPressed:           controlColorHov// Control background color (pressed)

    property color ctrlBorderColor:            borderColor    // x Control border color (default)
    property color ctrlBorderColorDisabled:    borderColor    // Control border color (disabled)
    property color ctrlBorderColorHighlighted: borderColor    // Control border color (highlighted)
    property color ctrlBorderColorHover:       borderColor    // Control border color (hovered)
    property color ctrlBorderColorPressed:     borderColor    // Control border color (pressed)
    property int   ctrlBorderWidth:            borderWidth    // x Control border width

    property color ctrlTextColor:              controlTextColor // x Control text color (default)
    property color ctrlTextColorDisabled:      disabledColor  // x Control text color (disabled)
    property color ctrlTextColorHighlighted:   controlTextColor // Control text color (highlighted)
    property color ctrlTextColorHover:         highlightColor // x Control text color (hovered)
    property color ctrlTextColorPressed:       activeColor    // x Control text color (pressed)
    property int   ctrlTextSize:               fontSizeSmall  // x Control text size

    property color ctrlIconColor:              controlTextColor // Control icon color (default)
    property color ctrlIconColorDisabled:      disabledColor    // Control icon color (disabled)
    property color ctrlIconColorHighlighted:   controlTextColor // Control icon color (highlighted)
    property color ctrlIconColorHover:         highlightColor   // Control icon color (hovered)
    property color ctrlIconColorPressed:       activeColor      // Control icon color (pressed)
    property int   ctrlIconSize:               16               // Control icon size


    //
    // Misc
    //

    property color debugColor: '#00bb00'


    //
    // Functions
    //

    function getControlColor(enabled, highlighted, hover, pressed)
    {
             if (!enabled)    return ctrlColorDisabled;
        else if (pressed)     return ctrlColorPressed;
        else if (hover)       return ctrlColorHover;
        else if (highlighted) return ctrlColorHighlighted;
        else                  return ctrlColor;
    }

    function getControlBorderColor(enabled, highlighted, hover, pressed)
    {
             if (!enabled)    return ctrlBorderColorDisabled;
        else if (pressed)     return ctrlBorderColorPressed;
        else if (hover)       return ctrlBorderColorHover;
        else if (highlighted) return ctrlBorderColorHighlighted;
        else                  return ctrlBorderColor;
    }

    function getControlTextColor(enabled, highlighted, hover, pressed)
    {
             if (!enabled)    return ctrlTextColorDisabled;
        else if (pressed)     return ctrlTextColorPressed;
        else if (hover)       return ctrlTextColorHover;
        else if (highlighted) return ctrlTextColorHighlighted;
        else                  return ctrlTextColor;
    }

    function getControlIconColor(enabled, highlighted, hover, pressed)
    {
             if (!enabled)    return ctrlIconColorDisabled;
        else if (pressed)     return ctrlIconColorPressed;
        else if (hover)       return ctrlIconColorHover;
        else if (highlighted) return ctrlIconColorHighlighted;
        else                  return ctrlIconColor;
    }
}
