
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
        // Get stage object
        var stage = getStageObject(path);

        if (stage)
        {
            return stage.getDescription();
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
        // Get stage object
        var stage = getStageObject(path);

        if (stage)
        {
            return stage.createStage(className, name);
        }

        // Error
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
        // Get stage object
        var stage = getStageObject(path);

        if (stage)
        {
            stage.removeStage(name);
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
        var stage = getStageObject(path);

        if (stage)
        {
            return stage.slotTypes();
        }

        return [];
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
        var stage = getStageObject(path);

        if (stage)
        {
            stage.createSlot(slotType, type, name);
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
        // Get stage object
        var stage = getStageObject(path);

        if (stage)
        {
            return stage.getConnections();
        }

        // Invalid stage
        return [];
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
        // Get stage object
        var stage = getStageObject('pipeline');

        if (stage)
        {
            stage.createConnection(from, to);
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
        // Get stage object
        var stage = getStageObject('pipeline');

        if (stage)
        {
            stage.removeConnection(to);
        }
    }

    // Internal functions

    function getStageObject(path)
    {
        // Begin with root object
        var stage = root;

        // Resolve path
        var names = path.split('.');
        for (var i=0; i<names.length; i++)
        {
            var name = names[i];

            if (name == 'pipeline' && i == 0) {
                stage = root;
            } else {
                stage = stage[name];
            }

            if (!stage) {
                return null;
            }
        }

        // Return stage
        return stage;
    }
}
