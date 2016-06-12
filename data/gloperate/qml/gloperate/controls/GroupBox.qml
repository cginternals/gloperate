
import QtQuick 2.0
import gloperate.base 1.0


/**
*  GroupBox
*
*  Panel with a border and title
*/
BaseItem
{
    id: item

    default property alias data: content.data

    property string title: ''

    implicitWidth:  rect.implicitWidth
    implicitHeight: rect.implicitHeight + label.implicitHeight / 2.0

    Label
    {
        id: label

        anchors.horizontalCenter: labelBackground.horizontalCenter
        anchors.verticalCenter:   labelBackground.verticalCenter
        z:                        2

        text:   item.title
        weight: Font.Bold
    }
    
    Rectangle
    {
        id: labelBackground

        anchors.left:       parent.left
        anchors.top:        parent.top
        anchors.leftMargin: Ui.style.dialogPadding
        width:              label.width + 2 * Ui.style.ctrlPadding
        height:             label.height
        z:                  1

        color: rect.color
    }
    
    Rectangle
    {
        id: rect

        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.top:    label.verticalCenter
        anchors.bottom: parent.bottom
        width:          parent.width

        implicitWidth:  content.implicitWidth
        implicitHeight: content.implicitHeight + content.anchors.topMargin

        color:        Ui.style.pageColor
        border.color: Ui.style.panelBorderColor
        border.width: Ui.style.panelBorderWidth
        radius:       Ui.style.panelBorderRadius
        
        Item
        {
            id: content

            anchors.fill:      parent
            anchors.margins:   Ui.style.panelPadding + rect.border.width
            anchors.topMargin: label.height/2.0

            implicitWidth:     childrenRect.width  + 2 * anchors.margins
            implicitHeight:    childrenRect.height + 2 * anchors.margins
        }
    }
}
