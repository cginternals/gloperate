
import QtQuick 2.0
import QtQuick.Controls 1.1 as Controls
import QtQuick.Controls.Styles 1.1
import gloperate.ui 1.0
import QtQuick.Controls.Private 1.0

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

    implicitWidth:  column.implicitWidth
    implicitHeight: column.implicitHeight

    Column
    {
        id: column

        width:   parent.width
        spacing: Ui.style.panelSpacing

        Label
        {
            width: parent.width

            text:  item.title
        }

        Rectangle
        {
            id: rect

            width:          parent.width
            implicitWidth:  content.implicitWidth
            implicitHeight: content.implicitHeight

            color:        Ui.style.panelColor
            border.color: Ui.style.panelBorderColor
            border.width: Ui.style.panelBorderWidth
            radius:       Ui.style.panelBorderRadius

            Item
            {
                id: content

                anchors.fill:    parent
                anchors.margins: Ui.style.panelPadding + rect.border.width

                implicitWidth:   childrenRect.width  + 2 * anchors.margins
                implicitHeight:  childrenRect.height + 2 * anchors.margins
            }
        }
    }
}
