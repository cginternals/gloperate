
import QtQuick 2.0


/**
*  InternalStage
*
*  Describes the state of an internal stage for (scripting and UI), only supports inputs
*/
Item
{
    id: stage

    signal connectionCreated(string sourcePath, string sourceSlot, string destPath, string destSlot)
    signal connectionRemoved(string path, string slot)

    property string name:     '' ///< Name of the stage (for display)
    property string path:     '' ///< Path of the stage
    property var inputs:      [] ///< Input configuration of the stage
    property var connections: {} ///< Connections to this stage

    property bool isVisibleInternal: true ///< Whether this stage should be visible in 'internal' mode
    property bool isVisibleExternal: true ///< Whether this stage should be visible in 'external' mode

    function configureComponents(stageItem) {} ///< Configure stage item in pipeline editor

    onConnectionCreated:
    {
        if (connections === undefined)
        {
            connections = {};
        }
        connections[destSlot] = sourcePath + '.' + sourceSlot;
    }

    onConnectionRemoved:
    {
        if (connections === undefined)
        {
            return;
        }

        delete connections[slot];
    }

    // Generate stage description with inputs, outputs, and stages
    function getDescription() {
        var inputNames = [];

        for (var i in inputs)
        {
            var input = inputs[i];
            inputNames.push(input.name);
        }

        return {
            name:    path,
            inputs:  inputNames,
            outputs: [],
            stages:  []
        };
    }

    // Generate stage connections
    function getConnections() {
        var connectionList = [];

        for (var slot in connections)
        {
            connectionList.push({from: connections[slot], to: path + '.' + slot});
        }

        return connectionList;
    }

}
