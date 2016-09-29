
import QtQuick 2.0
import QtQuick.Controls 1.0

import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  Pipeline
*
*  Representation of a pipeline
*/
BaseItem
{
    id: pipeline

    // Options
    property var    pipelineInterface: null ///< Interface for accessing the pipeline
    property string path:              ''   ///< Path in the pipeline hierarchy (e.g., 'pipeline')

    // Internals
    property var    stageItems:     null ///< Item cache
    property string hoveredElement: ''   ///< Path of element that is currently hovered
    property string selectedInput:  ''   ///< Path of selected input slot ('' if none)
    property string selectedOutput: ''   ///< Path of selected output slot ('' if none)
    property int    mouseX:         0;   ///< Current mouse position (X component)
    property int    mouseY:         0;   ///< Current mouse position (Y component)

    width:  stages.width
    height: stages.height

    /**
    *  Component that contains the template for a stage
    */
    property Component stageComponent: Stage
    {
        onXChanged: connectors.requestPaint();
        onYChanged: connectors.requestPaint();
    }

    /**
    *  Item for drawing the connectors
    */
    Connectors
    {
        id: connectors

        width:  stages.width
        height: stages.height

        pipelineInterface: pipeline.pipelineInterface
        path:              pipeline.path
        pipeline:          pipeline
    }

    // Main menu
    Menu
    {
        id: menu

        title: "Pipeline"

        property bool initialized: false

        function initialize()
        {
            if (initialized)
            {
                return;
            }

            var menu = addMenu('Add Stage');

            var types = pipelineInterface.getStageTypes();
            for (var i in types)
            {
                var type = types[i];

                var item = menu.addItem(type);

                var callbackFactory = function(type)
                {
                    return function()
                    {
                        pipeline.createStage(type, type);
                    };
                };

                item.triggered.connect(callbackFactory(type));
            }

            initialized = true;
        }
    }

    /**
    *  Background mouse area
    */
    MouseArea
    {
        anchors.fill: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled:    true

        onClicked:
        {
            // Deselect slots
            if (mouse.button == Qt.LeftButton)
            {
                pipeline.selectedInput  = '';
                pipeline.selectedOutput = '';
                connectors.requestPaint();
            }

            // Open context menu
            if (mouse.button == Qt.RightButton)
            {
                menu.initialize();
                menu.popup();
            }
        }

        onPositionChanged:
        {
            // Update mouse position
            pipeline.mouseX = mouse.x;
            pipeline.mouseY = mouse.y;

            // Draw new connection
            if (pipeline.selectedInput != '' || pipeline.selectedOutput != '')
            {
                connectors.requestPaint();
            }
        }
    }

    /**
    *  Item that contains the stages
    */
    Item
    {
        id: stages

        width:  childrenRect.x + childrenRect.width
        height: childrenRect.y + childrenRect.height
    }

    /**
    *  Load pipeline
    */
    onPathChanged:
    {
        // Clear old pipeline
        clearItems();

        // Exit if no pipeline is set
        if (path == '') {
            return;
        }

        // Get pipeline
        var pipelineDesc = pipelineInterface.getStage(pipeline.path);

        // Add stages
        var x = 200;
        var y = 150;

        for (var i in pipelineDesc.stages)
        {
            var name = pipelineDesc.stages[i];

            var stage = addStageItem(pipeline.path + '.' + name, name, x, y);

            x += stage.width + 20;
        }

        // Add pseudo stages for inputs and outputs of the pipeline itself
        addInputStageItem (pipeline.path, 'Inputs',    20, 150);
        addOutputStageItem(pipeline.path, 'Outputs', 1600, 150);

        // Do the layout
        computeLayout();

        // Redraw connections
        connectors.requestPaint();
    }

    /**
    *  Clear all items in the pipeline editor
    */
    function clearItems()
    {
        // Destroy all stages
        for (var i=0; i<stages.children.length; i++)
        {
            var stage = stages.children[i];
            stage.destroy();
        }

        // Reset item cache
        stageItems = {};
    }

    /**
    *  Add item for stage
    *
    *  @param[in] path
    *    Path in the pipeline hierarchy
    *  @param[in] name
    *    Name of stage
    *  @param[in] x
    *    X-position
    *  @param[in] y
    *    Y-position
    */
    function addStageItem(path, name, x, y)
    {
        // Create item for stage
        var stage = stageComponent.createObject(
            stages,
            {
                pipelineInterface: pipelineInterface,
                pipeline:          pipeline,
                x:                 x || 100,
                y:                 y || 100,
                name:              name || 'Stage'
            }
        );

        // Load stage
        stage.path = path;

        // Add to item cache
        stageItems[name] = stage;

        // Connect signal to remove the stage
        stage.closed.connect(function()
        {
            removeStage(path, name);
        });

        // Return item
        return stage;
    }

    /**
    *  Add item for input-stage
    *
    *  @param[in] path
    *    Path in the pipeline hierarchy
    *  @param[in] name
    *    Name of stage
    *  @param[in] x
    *    X-position
    *  @param[in] y
    *    Y-position
    */
    function addInputStageItem(path, name, x, y)
    {
        // Create item for stage
        var stage = stageComponent.createObject(
            stages,
            {
                pipelineInterface: pipelineInterface,
                pipeline:          pipeline,
                x:                 x || 100,
                y:                 y || 100,
                name:              name,
                color:             Ui.style.pipelineTitleColor2,
                includeInputs:     true,
                includeOutputs:    false,
                inverse:           true,
                allowClose:        false
            }
        );

        // Load stage
        stage.path = path;

        // Add to item cache
        stageItems[name] = stage;

        // Return item
        return stage;
    }

    /**
    *  Add item for output-stage
    *
    *  @param[in] path
    *    Path in the pipeline hierarchy
    *  @param[in] name
    *    Name of stage
    *  @param[in] x
    *    X-position
    *  @param[in] y
    *    Y-position
    */
    function addOutputStageItem(path, name, x, y)
    {
        // Create item for stage
        var stage = stageComponent.createObject(
            stages,
            {
                pipelineInterface: pipelineInterface,
                pipeline:          pipeline,
                x:                 x || 100,
                y:                 y  || 100,
                name:              name,
                color:             Ui.style.pipelineTitleColor2,
                includeInputs:     false,
                includeOutputs:    true,
                inverse:           true,
                allowClose:        false
            }
        );

        // Load stage
        stage.path = path;

        // Add to item cache
        stageItems[name] = stage;

        // Return item
        return stage;
    }

    /**
    *  Create new stage in pipeline
    *
    *  @param[in] path
    *    Path of pipeline
    *  @param[in] className
    *    Stage class
    *  @param[in] name
    *    Name of stage
    */
    function createStage(className, name)
    {
        // Create stage
        var realName =  pipelineInterface.createStage(pipeline.path, className, name);

        // Create stage item
        addStageItem(pipeline.path + '.' + realName, realName, 100, 100);
    }

    /**
    *  Remove stage from pipeline
    *
    *  @param[in] path
    *    Path of pipeline
    *  @param[in] name
    *    Name of stage
    */
    function removeStage(path, name)
    {
        // Remove stage item
        stageItems[name].destroy();
        delete stageItems[name];

        // Destroy stage
        pipelineInterface.removeStage(pipeline.path, name);

        // Redraw connections
        connectors.requestPaint();
    }

    /**
    *  Compute automatic layout for stages
    */
    function computeLayout()
    {
        // [TODO]
    }

    /**
    *  Get position of slot in pipeline
    *
    *  @param[in] path
    *    Path in pipeline hierarchy (e.g., 'pipeline.stage1.input1')
    */
    function getSlotPos(path)
    {
        // The path must start with the path to this pipeline, otherwise something is wrong
        var prefix = pipeline.path + '.';
        if (path.substr(0, prefix.length) == prefix)
        {
            // Get path relative to the pipeline
            var subPath = path.substr(prefix.length);
            var names = subPath.split('.');

            // If subpath has two components, it is a slot on a stage
            if (names.length == 2)
            {
                var stageName = names[0];
                var slotName  = names[1];

                // Get stage
                var stageItem = stageItems[stageName];
                if (stageItem)
                {
                    var pos = stageItem.getSlotPos(slotName);
                    return stageItem.mapToItem(pipeline, pos.x, pos.y);
                }
            }

            // If subpath has only one component, it is a slot on this pipeline
            else if (names.length == 1)
            {
                var slotName = names[0];

                // Get stage
                var inputsItem  = stageItems['Inputs'];
                var outputsItem = stageItems['Outputs'];

                if (inputsItem && outputsItem)
                {
                    var pos = inputsItem.getSlotPos(slotName);

                    if (pos)
                    {
                        return inputsItem.mapToItem(pipeline, pos.x, pos.y);
                    }

                    pos = outputsItem.getSlotPos(slotName);

                    if (pos)
                    {
                        return outputsItem.mapToItem(pipeline, pos.x, pos.y);
                    }
                }
            }
        }

        // Error in calling this function
        return null;
    }

    /**
    *  Called when mouse has entered a slot
    *
    *  @param[in] path
    *    Path in pipeline hierarchy (e.g., 'pipeline.stage1.input1')
    */
    function onSlotEntered(path)
    {
        hoveredElement = path;

        connectors.requestPaint();
    }

    /**
    *  Called when mouse has left a slot
    *
    *  @param[in] path
    *    Path in pipeline hierarchy (e.g., 'pipeline.stage1.input1')
    */
    function onSlotExited(path)
    {
        if (hoveredElement == path)
        {
            hoveredElement = '';
        }

        connectors.requestPaint();
    }

    /**
    *  Called when an input slot has been selected
    *
    *  @param[in] path
    *    Path in pipeline hierarchy (e.g., 'pipeline.stage1.input1')
    */
    function onInputSelected(path)
    {
        // If input slot is already connected, remove connection
        pipelineInterface.removeConnection(path);

        // Connection created
        if (path != '' && selectedOutput != '')
        {
            // Create connection
            pipelineInterface.createConnection(selectedOutput, path);

            // Reset selection
            selectedInput  = '';
            selectedOutput = '';
        }

        // Select one side
        else
        {
            // Select input slot
            selectedInput  = path;
            selectedOutput = '';
        }

        // Redraw connections
        connectors.requestPaint();
    }

    /**
    *  Called when an output slot has been selected
    *
    *  @param[in] path
    *    Path in pipeline hierarchy (e.g., 'pipeline.stage1.output1')
    */
    function onOutputSelected(path)
    {
        // Connection created
        if (path != '' && selectedInput != '')
        {
            // Create connection
            pipelineInterface.createConnection(path, selectedInput);

            // Reset selection
            selectedInput  = '';
            selectedOutput = '';
        }

        // Select one side
        else
        {
            // Select output slot
            selectedInput  = '';
            selectedOutput = path;
        }

        // Redraw connections
        connectors.requestPaint();
    }
}
