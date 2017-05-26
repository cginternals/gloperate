
import QtQuick 2.0


/**
*  GlOperatePipeline
*
*  Implementation of PipelineInterface for gloperate.
*/
Item
{
    id: pipelineInterface

    // Root pipeline object (accessed by 'pipeline')
    property var root: null

    /**
    *  Get list of available stage types
    *
    *
    *  @return
    *    [ 'StageType1', 'StageType2', ... ]
    */
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

    /**
    *  Get information of a stage
    *
    *  @param[in] path
    *    Path to stage (e.g., 'pipeline.stage1')
    *
    *  @return
    *    {
    *      name: 'StageName',
    *      inputs:  [ 'Input1',  ... ],
    *      outputs: [ 'Output1', ... ],
    *      stages:  [ 'Child1',  ... ]
    *    }
    */
    function getStage(path)
    {
        var info = root ? root.getDescription(path) : null;

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

    /**
    *  Get information of a slot
    *
    *  @param[in] path
    *    Path to slot (e.g., 'pipeline.stage1.in1')
    *
    *  @return
    *    {
    *      name:    'SlotName',
    *      type:    'int',
    *      value:   100,
    *      options: {}
    *    }
    */
    function getSlot(path)
    {
        var info = root ? root.getSlot(path) : null;

        if (info) {
            return info;
        } else {
            return {
                name:    '',
                type:    '',
                value:   null,
                options: {}
            };
        }
    }

    /**
    *  Set slot value
    *
    *  @param[in] path
    *    Path to slot (e.g., 'pipeline.stage1.in1')
    *  @param[in] value
    *    Value
    */
    function setSlotValue(path, value)
    {
        if (root)
        {
            root.setSlotValue(path, value);
        }
    }

    /**
    *  Create new stage
    *
    *  @param[in] path
    *    Path to pipeline (e.g., 'pipeline')
    *  @param[in] className
    *    Type of stage
    *  @param[in] name
    *    Desired name
    *
    *  @return
    *    Actual name of new stage
    */
    function createStage(path, className, name)
    {
        if (root)
        {
            return root.createStage(path, className, name);
        }
    }

    /**
    *  Remove stage from pipeline
    *
    *  @param[in] path
    *    Path to pipeline (e.g., 'pipeline')
    *  @param[in] name
    *    Stage name
    */
    function removeStage(path, name)
    {
        if (root)
        {
            root.removeStage(path, name);
        }
    }

    /**
    *  Get types of slots that can be created on a stage
    *
    *  @param[in] path
    *    Path to stage (e.g., 'pipeline.stage1')
    *
    *  @return
    *    Array of available slot types
    */
    function getSlotTypes(path)
    {
        var types = root ? root.slotTypes() : null;

        if (types) return types;
        else       return [];
    }

    /**
    *  Create slot on stage
    *
    *  @param[in] path
    *    Path to stage (e.g., 'pipeline.stage1')
    *  @param[in] slotType
    *    Slot type ('input' or 'output')
    *  @param[in] type
    *    Type (see getSlotTypes())
    *  @param[in] name
    *    Name of slot
    */
    function createSlot(path, slotType, type, name)
    {
        if (root)
        {
            root.createSlot(path, slotType, type, name);
        }
    }

    /**
    *  Get connections of slots on a stage
    *
    *  @param[in] path
    *    Path to stage (e.g., 'pipeline.stage1')
    *
    *  @return
    *    Array of connections ( e.g., [ { from: 'pipeline.stage1.out1', to: 'pipeline.stage2.in3' }, ... ] )
    */
    function getConnections(path)
    {
        var connections = root ? root.getConnections(path) : null;

        if (connections) return connections;
        else             return [];
    }

    /**
    *  Create connection between slots
    *
    *  @param[in] from
    *    Path to slot (e.g., 'pipeline.stage1.out1')
    *  @param[in] to
    *    Path to slot (e.g., 'pipeline.stage2.in1')
    */
    function createConnection(from, to)
    {
        if (root)
        {
            root.createConnection(from, to);
        }
    }

    /**
    *  Remove connection between slots
    *
    *  @param[in] to
    *    Path to slot (e.g., 'pipeline.stage2.in1')
    */
    function removeConnection(to)
    {
        if (root)
        {
            root.removeConnection(to);
        }
    }
}
