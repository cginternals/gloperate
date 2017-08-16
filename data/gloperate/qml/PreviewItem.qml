
import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0

import gloperate.rendering 1.0


Item
{
    id: previewItem

    property string path: '' ///< holds the path to the texture

    Rectangle
    {
        x:      100
        y:      100
        width:  200
        height: 200

        color:        'white'
        border.color: 'black'
        border.width: 1

        TextureItem
        {
            anchors.fill: parent
            anchors.margins: 1

            path: previewItem.path
        }

        MouseArea
        {
            anchors.fill: parent
            drag.target: parent
        }
    }
}
