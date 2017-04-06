
import QtQuick 2.0


/**
*  PipelineInterface
*
*  Generic interface for accessing a pipeline.
*  This interface can be implemented to access any type of pipeline.
*  The current implementation uses the gloperate pipeline as a backend.
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
        return pipeline.stageTypes;
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
        // Get stage object
        var stage = getStageObject(path);

        if (stage)
        {
            return stage;
        }

        // Invalid stage
        return {
            name:    '',
            inputs:  [],
            outputs: [],
            stages:  []
        };
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
        return {
            name:    'slot',
            type:    'int',
            value:   0,
            options: {}
        };
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
        return '';
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
        return pipeline.dataTypes;
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
        return pipeline.connections[path];
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
    }

    /**
    *  Remove connection between slots
    *
    *  @param[in] to
    *    Path to slot (e.g., 'pipeline.stage2.in1')
    */
    function removeConnection(to)
    {
    }

    // Internal functions

    function getStageObject(path)
    {
        return pipeline.stages[path];
    }

    property var pipeline: {
        'stageTypes' : [ 'Add', 'Substract', 'Multiply', 'FromVector', 'ToVector' ],

        'dataTypes' : [ 'float', 'vec3' ],

        'stages': {
            'pipeline': {
                name:    'pipeline',
                inputs:  [ 'v1', 'v2' ],
                outputs: [ 'normal' ],
                stages:  [ 'FromVector1', 'FromVector2', 'Multiply1', 'Multiply2', 'Substract1', 'ToVector' ]
            },

            'pipeline.FromVector1': {
                name:    'FromVector1',
                inputs:  [ 'v' ],
                outputs: [ 'x', 'y', 'z' ],
                stages:  []
            },

            'pipeline.FromVector2': {
                name:    'FromVector2',
                inputs:  [ 'v' ],
                outputs: [ 'x', 'y', 'z' ],
                stages:  []
            },

            'pipeline.Multiply1': {
                name:    'Multiply1',
                inputs:  [ 'a', 'b' ],
                outputs: [ 'p' ],
                stages:  []
            },

            'pipeline.Multiply2': {
                name:    'Multiply2',
                inputs:  [ 'a', 'b' ],
                outputs: [ 'p' ],
                stages:  []
            },

            'pipeline.Substract1': {
                name:    'Substract1',
                inputs:  [ 'a', 'b' ],
                outputs: [ 'd' ],
                stages:  []
            },

            'pipeline.ToVector': {
                name:    'ToVector',
                inputs:  [ 'x', 'y', 'z' ],
                outputs: [ 'v' ],
                stages:  []
            }
        },

        'connections': {
            'pipeline.FromVector1': [
                { from: 'pipeline.v1', to: 'pipeline.FromVector1.v' }
            ],

            'pipeline.FromVector2': [
                { from: 'pipeline.v2', to: 'pipeline.FromVector2.v' }
            ],

            'pipeline.Multiply1': [
                { from: 'pipeline.FromVector1.y', to: 'pipeline.Multiply1.a' },
                { from: 'pipeline.FromVector2.z', to: 'pipeline.Multiply1.b' }
            ],

            'pipeline.Multiply2': [
                { from: 'pipeline.FromVector1.z', to: 'pipeline.Multiply2.a' },
                { from: 'pipeline.FromVector2.y', to: 'pipeline.Multiply2.b' }
            ],

            'pipeline.Substract1': [
                { from: 'pipeline.Multiply1.p', to: 'pipeline.Substract1.a' },
                { from: 'pipeline.Multiply2.p', to: 'pipeline.Substract1.b' }
            ],

            'pipeline.ToVector': [
                { from: 'pipeline.Substract1.d', to: 'pipeline.ToVector.x' }
            ],

            'pipeline': [
                { from: 'pipeline.ToVector.v', to: 'pipeline.normal' }
            ]
        }
    }
}
