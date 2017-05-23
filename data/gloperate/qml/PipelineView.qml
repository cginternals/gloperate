
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0 as Editor

import gloperate.rendering 1.0


Item
{
    id: page

    signal closed()

    property var    pipelineInterface: null ///< Interface for communicating with the actual pipeline
    property string path:              ''   ///< Path to pipeline

    implicitWidth:  pipelineEditor.implicitWidth
    implicitHeight: pipelineEditor.implicitHeight

    Editor.PipelineEditor
    {
        id: pipelineEditor

        anchors.fill: parent

        pipelineInterface: page.pipelineInterface

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

                //path: 'DemoPipeline.TextureLoadStage.texture'
                path: 'DemoPipeline.FramebufferStage1.colorTexture'
            }

            MouseArea
            {
                anchors.fill: parent
                drag.target: parent
            }
        }
    }

    Component.onCompleted:
    {
        pipelineEditor.load(path);
    }
}
