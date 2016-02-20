
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
        anchors.top:    title.bottom
        anchors.bottom: parent.bottom
        width:          parent.width
    }
}
