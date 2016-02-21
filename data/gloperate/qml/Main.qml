
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
        x:      300
        y:      parent.height / 4.0
        width:  700
        height: parent.height / 2.0
    }
}
