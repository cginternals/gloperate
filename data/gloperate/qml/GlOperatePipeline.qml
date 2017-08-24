
import QtQuick 2.0


/**
*  GlOperatePipeline
*
*  Implementation of PipelineInterface for gloperate.
*/
Item
{
    id: pipelineInterface

    signal slotChanged(string path, string slot, var status)

    // Canvas scripting interface (accessed by 'root')
    property var canvas: null

    function getStageTypes()
    {
        var types = [];

        // List all components of type 'gloperate::Stage'
        var components = gloperate.components.components();
        for (var i in components)
        {
            var component = components[i];

            if (component.type == 'gloperate::Stage')
            {
                types.push(component.name);
            }
        }

        return types;
    }

    function getSlotTypes(path)
    {
        var types = canvas ? canvas.getSlotTypes(path) : null;

        if (types) return types;
        else       return [];
    }

    function createStage(path, name, type)
    {
        if (canvas)
        {
            return canvas.createStage(path, name, type);
        }
    }

    function removeStage(path, name)
    {
        if (canvas)
        {
            canvas.removeStage(path, name);
        }
    }

    function createSlot(path, slot, slotType, type)
    {
        if (canvas)
        {
            canvas.createSlot(path, slot, slotType, type);
        }
    }

    function getConnections(path)
    {
        var connections = canvas ? canvas.getConnections(path) : null;

        if (connections) return connections;
        else             return [];
    }

    function createConnection(sourcePath, sourceSlot, destPath, destSlot)
    {
        if (canvas)
        {
            canvas.createConnection(sourcePath, sourceSlot, destPath, destSlot);
        }
    }

    function removeConnection(path, slot)
    {
        if (canvas)
        {
            canvas.removeConnection(path, slot);
        }
    }

    function getStage(path)
    {
        var info = canvas ? canvas.getStage(path) : null;

        if (info) {
            return info;
        } else {
            return {
                name:    '',
                inputs:  [],
                outputs: [],
                stages:  []
            };
        }
    }

    function getSlot(path, slot)
    {
        var info = canvas ? canvas.getSlot(path, slot) : null;

        if (info) {
            return info;
        } else {
            return {
                name:  '',
                type:  '',
                value: null,
            };
        }
    }

    function getValue(path, slot)
    {
        if (canvas)
        {
            canvas.getValue(path, slot);
        }
    }

    function setValue(path, slot, value)
    {
        if (canvas)
        {
            canvas.setValue(path, slot, value);
        }
    }
}
