
import QtQuick 2.0
import QtQuick.Controls 1.1 as Controls
import QtQuick.Controls.Styles 1.1
import gloperate.base 1.0


/**
*  RadioButton
*
*  Radio button wrapper
*/
Control
{
    id: item

    implicitWidth:  rb.implicitWidth
    implicitHeight: rb.implicitHeight

    property alias activeFocusOnPress: rb.activeFocusOnPress
    property alias checked:            rb.checked
    property alias exclusiveGroup:     rb.exclusiveGroup
    property alias hovered:            rb.hovered
    property alias pressed:            rb.pressed
    property alias style:              rb.style
    property alias text:               rb.text

    Controls.RadioButton
    {
        id: rb

        style: RadioButtonStyle
        {
            label: Label
            {
                text: rb.text
            }
        }

        onClicked:
        {
            item.clicked();
        }
    }
}
