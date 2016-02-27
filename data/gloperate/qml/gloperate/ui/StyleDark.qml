
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

    property color borderColor:     primaryColor
    property int   borderWidth:     1

    // Fonts
    property int   fontSizeHuge:    32
    property int   fontSizeLarge:   20
    property int   fontSizeMedium:  16
    property int   fontSizeSmall:   14
    property int   fontSizeTiny:    12

    // Padding sizes
    property real  paddingLarge:    12
    property real  paddingMedium:   6
    property real  paddingSmall:    3


    //
    // Generic style definition (to be used by specific items)
    //

    // Main page
    property color pageColor:                  '#ffffff'      // Page background
    property real  pagePadding:                0              // Page padding
    property real  pageSpacing:                0              // Page spacing

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
    property color pageLinkColorHighlighted:   primaryColor   // Page link color (highlighted)
    property color pageLinkColorPressed:       primaryColor   // Page link color (pressed)

    property color pageIconColor:              primaryColor   // Page icon color
    property int   pageIconSize:               64             // Page icon size

    // Panel
    property color panelColor:                 '#000000'      // Panel background
    property color panelBorderColor:           borderColor    // Panel border color
    property int   panelBorderWidth:           borderWidth    // Pabel border width
    property real  panelPadding:               0              // Panel padding
    property real  panelSpacing:               0              // Panel spacing

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
    property color panelLinkColorHighlighted:  primaryColor   // Panel link color (highlighted)
    property color panelLinkColorPressed:      primaryColor   // Panel link color (pressed)

    property color panelIconColor:             primaryColor   // Panel icon color
    property int   panelIconSize:              64             // Panel icon size

    // Controls
    property real  ctrlPadding:                0              // Control padding
    property real  ctrlSpacing:                0              // Control padding

    property color ctrlColor:                  '#000000'      // Control background color (default)
    property color ctrlColorDisabled:          '#000000'      // Control background color (disabled)
    property color ctrlColorHighlighted:       '#000000'      // Control background color (highlighted)
    property color ctrlColorPressed:           '#000000'      // Control background color (pressed)

    property color ctrlBorderColor:            '#000000'      // Control border color (default)
    property color ctrlBorderColorDisabled:    '#000000'      // Control border color (disabled)
    property color ctrlBorderColorHighlighted: '#000000'      // Control border color (highlighted)
    property color ctrlBorderColorPressed:     '#000000'      // Control border color (pressed)
    property int   ctrlBorderWidth:            borderWidth    // Control border width

    property color ctrlTextColor:              primaryColor   // Control text color (default)
    property color ctrlTextColorDisabled:      '#000000'      // Control text color (disabled)
    property color ctrlTextColorHighlighted:   '#000000'      // Control text color (highlighted)
    property color ctrlTextColorPressed:       '#000000'      // Control text color (pressed)
    property int   ctrlTextSize:               fontSizeSmall  // Control text size

    property color ctrlIconColor:              primaryColor   // Control icon color (default)
    property color ctrlIconColorDisabled:      '#000000'      // Control icon color (disabled)
    property color ctrlIconColorHighlighted:   '#000000'      // Control icon color (highlighted)
    property color ctrlIconColorPressed:       '#000000'      // Control icon color (pressed)
    property int   ctrlIconSize:               32             // Control icon size
}
