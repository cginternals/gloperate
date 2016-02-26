
import QtQuick 2.0
import gloperate.rendering 1.0
import gloperate.ui 1.0


Item
{
    Label
    {
        id: title

        anchors.top:              parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text:                     'Hello World'
    }

    RenderItem
    {
        id: item1

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

    RenderItem
    {
        id: item2

        x:      100
        y:      100
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
