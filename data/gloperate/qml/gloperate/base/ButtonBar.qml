
import QtQuick 2.0
import gloperate.base 1.0


/**
*  ButtonBar
*
*  Horizontal list of buttons
*/
BaseItem
{
    id: item

    // List of items that will be displayed:
    //   [ {name: 'customId', text: 'Displayed text', icon: 'icon.png', enabled: true}, ... ]
    property variant items: []

    // Called when one of the items has been clicked
    signal itemClicked(string menu, string name)

    implicitWidth:  row.implicitWidth
    implicitHeight: row.implicitHeight
    clip:           false

    Row
    {
        id: row

        anchors.top:  parent.top
        anchors.left: parent.left
        spacing:      Ui.style.pageSpacing

        Repeater
        {
            model: item.items.length

            Dropdown
            {
                id: subMenu

                property string name: item.items[index].name

                text:    item.items[index].text
                icon:    item.items[index].icon
                enabled: item.items[index].hasOwnProperty('enabled') ? item.items[index].enabled : true
                items:   item.items[index].hasOwnProperty('items')   ? item.items[index].items   : []

                onClicked:
                {
                    item.itemClicked(subMenu.name, '');
                }

                onItemClicked:
                {
                    item.itemClicked(subMenu.name, name);
                }
            }
        }
    }
}
