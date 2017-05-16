
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


Item
{
    id: item

    signal clicked()

    property color color:         '#0000ff'
    property color hoverColor:    '#8888ff'
    property color selectedColor: '#ffbb44'
    property bool  selected:      false
    property alias text:          label.text

    implicitWidth:  label.implicitWidth
    implicitHeight: label.implicitHeight

    MouseArea
    {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true

        onClicked: item.clicked();
    }

    Label
    {
        id: label

        anchors.fill: parent

        color:     mouseArea.containsMouse ? item.hoverColor : (item.selected ? item.selectedColor : item.color)
        font.bold: item.selected
    }
}
