
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0 as Editor


Item
{
    id: page

    signal closed()

    property var pipelineInterface: null ///< Interface for communicating with the actual pipeline

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
    }

    Component.onCompleted:
    {
        pipelineEditor.load();
    }
}
