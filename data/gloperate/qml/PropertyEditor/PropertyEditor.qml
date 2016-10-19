
import QtQuick 2.0
import QtQuick.Layouts 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.PipelineEditor 1.0


BaseItem
{
    id: item

    property var    pipelineInterface: null ///< Interface for communicating with the actual pipeline
    property string path:              ''   ///< Path to pipeline or stage (e.g., 'pipeline')

    implicitWidth:  grid.implicitWidth
    implicitHeight: grid.implicitHeight

    property int numProperties: 10
    property var properties:    []

    GridLayout
    {
        id: grid

        //width: parent.width

        rows: numProperties
        flow: GridLayout.TopToBottom

        rowSpacing:    Ui.style.spacingMedium
        columnSpacing: Ui.style.spacingLarge

        Repeater
        {
            model: item.numProperties

            delegate: Label
            {
                text: item.properties[index] || ''
            }
        }

        Repeater
        {
            id: repeater

            model: item.numProperties

            delegate: ValueEdit
            {
                pipelineInterface: item.pipelineInterface
                path:              item.path + '.' + item.properties[index]

                onPathChanged: update();
            }
        }
    }

    function update()
    {
        // Get stage info
        var stage = pipelineInterface.getStage(path);

        var num = stage.inputs.length || 0;

        var names = [];

        for (var i=0; i<num; i++)
        {
            names.push(stage.inputs[i]);
        }

        item.numProperties = num;
        item.properties    = names;
    }
}
