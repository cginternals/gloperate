
import QtQuick 2.0
import GLOperate 1.0


Item
{
    Text
    {
        id: title

        anchors.top:              parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text:                     'Hello World'
    }

    RenderItem
    {
        x:      0
        y:      0
        width:  700
        height: parent.height / 2.0

        MouseArea
        {
            anchors.fill: parent
            drag.target:  parent
        }

        Rectangle
        {
            anchors.fill: parent
            color:        'transparent'
            border.width: 1
            border.color: 'black'
        }
    }
}
