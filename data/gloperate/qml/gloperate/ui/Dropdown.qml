
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  Dropdown
*
*  A push button that opens a list of options
*/
Button
{
    id: item

    // List of items that will be displayed:
    //   [ { name: 'customId', text: 'Displayed text', icon: 'icon.png' }, ... ]
    property variant items: []

    // Called when one of the items has been clicked
    signal itemClicked(string name)

    implicitWidth: Math.max(buttonWidth, column.maxWidth)

    Column
    {
        id: column

        visible: false

        anchors.top:       parent.bottom
        anchors.left:      parent.left
        anchors.topMargin: Ui.style.pageSpacing
        spacing:           Ui.style.pageSpacing

        readonly property real maxWidth:
        {
            var max = 0.0;

            for (var i=0; i<column.children.length; i++)
            {
                var item = column.children[i];
                max = Math.max(max, item.implicitWidth);
            }

            return max;
        }

        Repeater
        {
            model: item.items.length

            Button
            {
                width: item.width
                text:  item.items[index].text
                icon:  item.items[index].icon

                onClicked:
                {
                    item.itemClicked(item.items[index].name);
                }
            }
        }
    }

    onClicked:
    {
        column.visible = !column.visible;
        item.highlighted = column.visible;
    }
}
