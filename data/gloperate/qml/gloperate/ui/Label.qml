
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Label
*
*  Displays a text with default style
*/
BaseItem
{
    id: item

    implicitWidth:  label.implicitWidth
    implicitHeight: label.implicitHeight

    property alias text: label.text

    Text
    {
        id: label

        anchors.fill:   parent

        color:          Ui.style.pageTextColor
        font.pixelSize: Ui.style.pageTextSize
        wrapMode:       Text.Wrap
    }
}
