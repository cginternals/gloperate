
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

    // Main colors
    property color primaryColor:     '#000000'
    property color secondaryColor:   '#777777'
    property color highlightColor:   '#4078c0'
    property color pressedColor:     '#6098e0'
    property color disabledColor:    '#999999'

    // Panels and controls
    property color backgroundColor:  '#ffffff'
    property color windowColor:      '#dddddd'
    property color windowColorAlt:   '#aaaaaa'
    property color controlColor:     '#ffffff'
    property color controlColorHl:   '#d0d0d0'
    property color controlColorHov:  '#f0f0f0'
    property color controlTextColor: '#222222'
    property color fieldColor:       '#ffffff'
    property color progressColor:    '#ADD8E6'

    // Borders
    property color borderColor:     '#888888'
    property int   borderWidth:     1
    property real  borderRadius:    3

    // Forms
    property real  formHeight:      24

    // Fonts
    property int   fontSizeHuge:    32
    property int   fontSizeLarge:   20
    property int   fontSizeMedium:  16
    property int   fontSizeSmall:   14
    property int   fontSizeTiny:    12

    // Padding sizes
    property real  paddingHuge:     50
    property real  paddingLarge:    12
    property real  paddingMedium:   8
    property real  paddingSmall:    4

    // Icon sizes
    property real  iconSizeLarge:   64
    property real  iconSizeMedium:  32
    property real  iconSizeSmall:   16


    //
    // Generic style definition (to be used by items)
    //

    // Misc
    property color debugColor:                 '#00bb00'        // Debug color

    // Main page
    property color pageColor:                  backgroundColor  // Page background color
    property real  pagePadding:                paddingSmall     // Page padding (space between border and page)
    property real  pageSpacing:                paddingSmall     // Page spacing (space between items on the page)

    // Panels
    property color panelColor:                 windowColor      // Panel background color
    property color panelColorAlt:              windowColorAlt   // Panel alternative background color
    property color panelBorderColor:           borderColor      // Panel border color
    property int   panelBorderWidth:           borderWidth      // Panel border width
    property real  panelBorderRadius:          borderRadius     // Panel border radius (round edges)
    property real  panelPadding:               paddingSmall     // Panel padding (space between border and items)
    property real  panelSpacing:               paddingMedium    // Panel spacing (space between items on a panel)

    // Panel items
    property color panelItemColor:             windowColorAlt   // Panel item color (default)
    property color panelItemColorDisabled:     controlColor     // Panel item color (disabled)
    property color panelItemColorHighlighted:  controlColorHl   // Panel item color (highlighted)
    property color panelItemColorHover:        controlColorHov  // Panel item color (hovered)
    property color panelItemColorPressed:      controlColorHl   // Panel item color (pressed)

    // Controls
    property color ctrlColor:                  controlColor     // Control background color (default)
    property color ctrlColorDisabled:          controlColor     // Control background color (disabled)
    property color ctrlColorHighlighted:       controlColorHl   // Control background color (highlighted)
    property color ctrlColorHover:             controlColorHov  // Control background color (hovered)
    property color ctrlColorPressed:           controlColorHl   // Control background color (pressed)

    property color ctrlBorderColor:            borderColor      // Control border color (default)
    property color ctrlBorderColorDisabled:    borderColor      // Control border color (disabled)
    property color ctrlBorderColorHighlighted: borderColor      // Control border color (highlighted)
    property color ctrlBorderColorHover:       borderColor      // Control border color (hovered)
    property color ctrlBorderColorPressed:     borderColor      // Control border color (pressed)

    property int   ctrlBorderWidth:            borderWidth      // Control border width
    property real  ctrlRadius:                 borderRadius     // Control border radius (round edges)
    property real  ctrlPadding:                paddingSmall     // Control padding (space between border and items)
    property real  ctrlSpacing:                paddingSmall     // Control spacing (space between items on the control)

    property color ctrlTextColor:              controlTextColor // Control text color (default)
    property color ctrlTextColorDisabled:      disabledColor    // Control text color (disabled)
    property color ctrlTextColorHighlighted:   primaryColor     // Control text color (highlighted)
    property color ctrlTextColorHover:         highlightColor   // Control text color (hovered)
    property color ctrlTextColorPressed:       pressedColor     // Control text color (pressed)
    property int   ctrlTextSize:               fontSizeMedium   // Control text size

    property color ctrlIconColor:              primaryColor     // Control icon color (default)
    property color ctrlIconColorDisabled:      disabledColor    // Control icon color (disabled)
    property color ctrlIconColorHighlighted:   primaryColor     // Control icon color (highlighted)
    property color ctrlIconColorHover:         highlightColor   // Control icon color (hovered)
    property color ctrlIconColorPressed:       pressedColor     // Control icon color (pressed)
    property int   ctrlIconSize:               iconSizeSmall    // Control icon size

    // Text
    property color textColor:                  primaryColor     // Text color (default)
    property color textColorSecondary:         secondaryColor   // Text color (secondary texts)
    property color textColorTitle:             highlightColor   // Text color (title texts)
    property int   textSize:                   fontSizeMedium   // Text size (default)
    property int   textSizeSecondary:          fontSizeMedium   // Text size (secondary texts)
    property int   textSizeTitle:              fontSizeMedium   // Text size (title texts)

    // Links
    property color linkColor:                  primaryColor     // Link color (default)
    property color linkColorDisabled:          disabledColor    // Link color (disabled)
    property color linkColorHover:             highlightColor   // Link color (hovered)
    property color linkColorPressed:           pressedColor     // Link color (pressed)

    // Dialogs
    property real  dialogPadding:              paddingMedium    // Padding between window and content

    // Forms
    property real  formControlHeight:          formHeight       // Height of form controls (e.g., input fields)

    // Pipeline editor
    property color pipelineStageColor:          '#b6bfc8'        // Background color of stages
    property int   pipelineStageRadius:         4                // Radius of stage rectangle
    property color pipelineTitleColor:          '#4f9e71'        // Background color of stage titles
    property color pipelineTitleColor2:         '#fdca00'        // Background color of stage titles
    property color pipelineTextColor:           primaryColor     // Color of texts in stages
    property color pipelineTitleTextColor:      primaryColor     // Color of values in stages
    property color pipelineLineColor:           primaryColor     // Color of lines, connections, etc.
    property int   pipelineLineWidth:           borderWidth      // Width of lines
    property color pipelineSlotColor:           panelColor       // Background color of slots
    property color pipelineSlotInvalidColor:    '#ff8888'        // Background color of invalid slots
    property color pipelineSlotRequiredColor:   '#00ff00'        // Line color of required slots
    property int   pipelineSlotRequiredBorder:  2                // Border width of required slots
    property real  pipelineSlotSize:            formHeight * 1.2 // Height of input/output slots
    property real  pipelineConnectorSize:       formHeight * 0.8 // Diameter of connectors
    property color pipelineConnectorColorParam: "#fdca00"        // Color of parameter connectors
    property color pipelineConnectorColorIn:    "#ffffff"        // Color of input connectors
    property color pipelineConnectorColorOut:   "#cafd00"        // Color of output connectors
    property real  pipelinePadding:             50               // Space between outer and inner pipelines
    property color pipelineColorDefault:        '#000000'        // Color of connections
    property color pipelineColorHighlighted:    '#6688c8'        // Color of connections when highlighted
    property color pipelineColorSelected:       '#c83366'        // Color of connections when selected


    //
    // Functions
    //

    function getPanelItemColor(enabled, highlighted, hover, pressed)
    {
             if (!enabled)    return panelItemColorDisabled;
        else if (pressed)     return panelItemColorPressed;
        else if (hover)       return panelItemColorHover;
        else if (highlighted) return panelItemColorHighlighted;
        else                  return panelItemColor;
    }

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

    function getLinkColor(enabled, hover, pressed)
    {
             if (!enabled)    return linkColorDisabled;
        else if (pressed)     return linkColorPressed;
        else if (hover)       return linkColorHover;
        else                  return linkColor;
    }
}
