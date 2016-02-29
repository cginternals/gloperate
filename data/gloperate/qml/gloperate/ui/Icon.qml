
import QtQuick 2.0
import QtGraphicalEffects 1.0
import gloperate.ui 1.0


/**
*  Icon
*
*  Displays an icon with an optional text label
*/
Control
{
    id: item

    property string icon:       ''
    property real   iconWidth:  Ui.style.ctrlIconSize
    property real   iconHeight: Ui.style.ctrlIconSize
    property color  color:      Ui.style.getControlIconColor(enabled, highlighted, hovered, pressed)

    implicitWidth:  icon.width
    implicitHeight: icon.height

    ColorOverlay
    {
        anchors.fill: parent

        source: icon
        color:  item.color
    }

    Image
    {
        id: icon

        width:   item.iconWidth
        height:  item.iconHeight
        source:  item.icon != '' ? '../../../icons/' + item.icon : ''
        visible: false
    }
}
