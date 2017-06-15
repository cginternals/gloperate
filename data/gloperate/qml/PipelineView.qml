
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0 as Editor

import gloperate.rendering 1.0


Item
{
    id: page

    signal closed()

    property var    properties: null ///< Interface for communicating with the actual properties
    property string path:       ''   ///< Path to pipeline

    implicitWidth:  pipelineEditor.implicitWidth
    implicitHeight: pipelineEditor.implicitHeight

    Editor.PipelineEditor
    {
        id: pipelineEditor

        anchors.fill: parent

        properties: page.properties

        Button
        {
            text: 'Close'

            anchors.top:     parent.top
            anchors.left:    parent.left
            anchors.margins: Ui.style.paddingMedium

            onClicked:
            {
                page.closed();
            }
        }

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

                path: 'ShapeDemo.Framebuffer.colorTexture'
            }

            MouseArea
            {
                anchors.fill: parent
                drag.target: parent
            }
        }
    }

    onVisibleChanged:
    {
        if (visible)
        {
            pipelineEditor.load(path);
        }
    }
}
