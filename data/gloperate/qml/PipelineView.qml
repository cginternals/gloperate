
import QtQuick 2.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0 as Editor

import gloperate.rendering 1.0


Item
{
    id: page

    signal closed()
    signal toggled()

    property var    properties:   null ///< Interface for communicating with the actual properties
    property string path:         ''   ///< Path to pipeline
    property string toggleButton: ''   ///< Description of the toggle state button

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

        Button
        {
            text: page.toggleButton
            visible: page.toggleButton === "" ? false : true

            anchors.top:     parent.top
            anchors.right:   parent.right
            anchors.margins: Ui.style.paddingMedium

            onClicked:
            {
                page.toggled();
            }
        }
    }

    function load()
    {
        if (pipelineEditor.loaded)
        {
            return;
        }

        pipelineEditor.load(path);

        configureInternalStages(properties.getInternalStages());
    }

    function update()
    {
        pipelineEditor.update();

        configureInternalStages(properties.getInternalStages());
    }

    function configureInternalStages(internalStages)
    {
        for (var stageName in internalStages)
        {
            var stageDefinition = internalStages[stageName];
            var stageObject = pipelineEditor.pipeline.stageItems[stageDefinition.name];

            stageDefinition.configureComponents(stageObject);
        }
    }
}
