
import QtQuick 2.0

import QmlToolbox.Base 1.0

import gloperate.rendering 1.0


Item
{
    id: previewItem

    property string path: '' ///< holds the path to the texture

    implicitWidth:  200
    implicitHeight: 180

    Rectangle
    {
        anchors.fill: parent
        anchors.leftMargin: Ui.style.pipelineSlotSize
        anchors.rightMargin: Ui.style.pipelineSlotSize / 2

        color:        'white'
        border.color: 'black'
        border.width: 1

        TextureItem
        {
            anchors.fill: parent
            anchors.margins: 1

            path: previewItem.path
        }
    }
}
