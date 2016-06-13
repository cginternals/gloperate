
import QtQuick 2.0
import gloperate.base 1.0


/**
*  Label
*
*  Displays a text with default style
*/
Control
{
    id: item

    property alias text:                label.text
    property alias color:               label.color
    property alias linkColor:           label.linkColor
    property alias size:                label.font.pixelSize
    property alias wrapMode:            label.wrapMode
    property alias verticalAlignment:   label.verticalAlignment
    property alias horizontalAlignment: label.horizontalAlignment
    property alias weight:              label.font.weight

    implicitWidth:  label.implicitWidth
    implicitHeight: label.implicitHeight

    Text
    {
        id: label

        anchors.fill: parent

        color:             Ui.style.textColor
        linkColor:         Ui.style.linkColor
        font.pixelSize:    Ui.style.textSize
        verticalAlignment: Text.AlignVCenter
        wrapMode:          Text.Wrap
    }
}
