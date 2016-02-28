
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Label
*
*  Displays a text with default style
*/
Control
{
    id: item

    property alias text:  label.text
    property alias color: label.color
    property alias size:  label.font.pixelSize

    implicitWidth:  label.implicitWidth
    implicitHeight: label.implicitHeight

    Text
    {
        id: label

        anchors.fill: parent

        color:             Ui.style.pageTextColor
        font.pixelSize:    Ui.style.pageTextSize
        verticalAlignment: Text.AlignVCenter
        wrapMode:          Text.Wrap
    }
}
