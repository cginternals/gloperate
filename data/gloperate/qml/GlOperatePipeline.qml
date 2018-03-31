
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

    property var canvas:         null ///< Canvas scripting interface (accessed by 'root')
    property var internalStages: {}   ///< Store stages only accessible for scripting and UI
    property var internalTypes:  null ///< Stores internal stage types, lazy initialize
    property string editor:      ''   ///< State of editor

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

        types = types.concat(Object.keys(getInternalTypes()));

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
        if (getInternalTypes().hasOwnProperty(type))
        {
            return createInternalStage(path, name, type);
        }

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

        if (!connections)
        {
            return [];
        }

        if (path === "root")
        {
            for (var internalPath in internalStages)
            {
                if (isVisible(internalStages[internalPath]))
                {
                    connections = connections.concat(internalStages[internalPath].getConnections());
                }
            }
        }

        return connections;
    }

    function createConnection(sourcePath, sourceSlot, destPath, destSlot)
    {
        for (var internalPath in internalStages)
        {
            if (destPath === internalPath)
            {
                internalStages[internalPath].connectionCreated(sourcePath, sourceSlot, destPath, destSlot);
            }
        }

        if (canvas)
        {
            canvas.createConnection(sourcePath, sourceSlot, destPath, destSlot);
        }
    }

    function removeConnection(path, slot)
    {
        for (var internalPath in internalStages)
        {
            if (path === internalPath)
            {
                internalStages[internalPath].connectionRemoved(path, slot);
            }
        }

        if (canvas)
        {
            canvas.removeConnection(path, slot);
        }
    }

    function getStage(path)
    {
        var info = canvas ? canvas.getStage(path) : null;

        if (info && path === "root")
        {
            for (var internalPath in internalStages)
            {
                var stage = internalStages[internalPath];
                if (isVisible(stage))
                {
                    info.stages.push(stage.name);
                }
            }
        }

        for (var internalPath in internalStages)
        {
            if (path === internalPath)
            {
                return internalStages[internalPath].getDescription();
            }
        }

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

    // Functions regarding internal stages
    function isVisible(stage)
    {
        return editor === 'internal' ? stage.isVisibleInternal : stage.isVisibleExternal;
    }

    function getInternalTypes()
    {
        if (internalTypes === null)
        {
            initializeInternalTypes();
        }
        return internalTypes;
    }

    function getInternalStages()
    {
        var stages = {};

        for (var internalPath in internalStages)
        {
            var stage = internalStages[internalPath];

            if (isVisible(stage))
            {
                stages[internalPath] = stage;
            }
        }

        return stages;
    }

    function clearInternalStages()
    {
        internalStages = {};
    }

    function createInternalStage(path, name, type)
    {
        var realName = name;

        if (internalStages.hasOwnProperty(path + "." + name))
        {
            // name already taken, add a number
            var i = 2;
            while (internalStages.hasOwnProperty(path + "." + name + i))
            {
                i += 1;
            }
            realName = name + i;
        }

        var stageComponent = getInternalTypes()[type];

        var newStage = stageComponent.createObject(pipelineInterface, {});
        var stage = newStage.stageDefinition;

        stage.name = realName;
        stage.path = path + "." + realName;

        internalStages[stage.path] = stage;

        return realName;
    }

    function initializeInternalTypes()
    {
        internalTypes = {};

        internalTypes['PreviewStage'] = previewStageComponent;
    }

    property Component previewStageComponent : PreviewStage
    {
        id: previewStage
    }
}
