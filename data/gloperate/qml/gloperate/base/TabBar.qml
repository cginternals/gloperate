
import QtQuick 2.0
import gloperate.base 1.0


/**
*  TabBar
*
*  Container that displays a list of tabs
*/
BaseItem
{
    id: item

    // List of items that will be displayed:
    //   [ {name: 'customId', text: 'Displayed text', icon: 'icon.png', enabled: true}, ... ]
    property variant items: []

    // Name of the currently selected item
    property string selectedItem: ''

    // Called when a tab has been selected
    signal tabSelected(string name)

    // Set currently selected tab
    function selectTab(name)
    {
        selectedItem = name;
        tabSelected(name);
    }

    implicitWidth:  row.implicitWidth
    implicitHeight: row.implicitHeight

    Row
    {
        id: row

        anchors.top:  parent.top
        anchors.left: parent.left
        spacing:      Ui.style.pageSpacing

        Repeater
        {
            model: item.items.length

            Button
            {
                property string name: item.items[index].name

                text:        item.items[index].text
                icon:        item.items[index].icon
                enabled:     item.items[index].hasOwnProperty('enabled') ? item.items[index].enabled : true
                highlighted: selectedItem == name

                onClicked:
                {
                    item.selectTab(name);
                }
            }
        }
    }
}
