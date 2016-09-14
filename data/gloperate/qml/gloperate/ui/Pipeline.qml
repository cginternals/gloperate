
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

    width:  stages.width
    height: stages.height

    property string path: '' ///< Path in the pipeline hierarchy (e.g., 'DemoPipeline.SubPipeline')

    // Internals
    property var    stageItems:     null ///< Item cache
    property string hoveredElement: ''   ///< Path of element that is currently hovered
    property string selectedInput:  ''   ///< Path of selected input slot ('' if none)
    property string selectedOutput: ''   ///< Path of selected output slot ('' if none)
    property int    mouseX:         0;   ///< Current mouse position (X component)
    property int    mouseY:         0;   ///< Current mouse position (Y component)


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

        pipeline: pipeline
        stages:   stages
    }

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

            var components = gloperate.components.components();
            for (var i in components)
            {
                var component = components[i];

                if (component.type == 'gloperate::Stage')
                {
                    var item = menu.addItem(component.name);

                    var callbackFactory = function(type)
                    {
                        return function()
                        {
                            pipeline.createStage(type, type);
                        };
                    };

                    item.triggered.connect(callbackFactory(component.name));
                }
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
            if (mouse.button == Qt.LeftButton)
            {
                pipeline.selectedInput  = '';
                pipeline.selectedOutput = '';
                connectors.requestPaint();
            }

            if (mouse.button == Qt.RightButton)
            {
                menu.initialize();
                menu.popup();
            }
        }

        onPositionChanged:
        {
            pipeline.mouseX = mouse.x;
            pipeline.mouseY = mouse.y;

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
    *  Load pipeline from pipeline viewer
    */
    onPathChanged:
    {
        // Clear old pipeline
        clear();

        // Exit if no pipeline is set
        if (path == '')
            return;

        // Get pipeline
        var pl     = getStage(pipeline.path);
        var plDesc = pl.getDescription();

        // Add stages
        var x = 200;
        var y = 150;

        for (var i in plDesc.stages)
        {
            var name = plDesc.stages[i];

            // Get stage
            var st     = pl[name];
            var stDesc = st.getDescription();

            // Create stage in editor
            var stage = addStage(pipeline.path + '.' + name, name, x, y);

            x += stage.width + 20;
        }

        // Add pseudo stages for inputs and outputs of the pipeline itself
        addInputStage (pipeline.path, 'Inputs',    20, 150);
        addOutputStage(pipeline.path, 'Outputs', 1600, 150);

        // Do the layout
        computeLayout();

        // Redraw connections
        connectors.requestPaint();
    }

    function clear()
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

    function addInputStage(path, name, x, y)
    {
        // Create item for stage
        var item = stageComponent.createObject(
            stages,
            {
                pipeline:       pipeline,
                x:              x || 100,
                y:              y || 100,
                name:           name,
                color:          Ui.style.pipelineTitleColor2,
                includeInputs:  true,
                includeOutputs: false,
                inverse:        true,
                allowClose:     false
            }
        );

        // Load stage
        item.path = path;

        // Add to item cache
        stageItems[name] = item;

        // Return item
        return item;
    }

    function addOutputStage(path, name, x, y)
    {
        // Create item for stage
        var item = stageComponent.createObject(
            stages,
            {
                pipeline:       pipeline,
                x:              x || 100,
                y:              y || 100,
                name:           name,
                color:          Ui.style.pipelineTitleColor2,
                includeInputs:  false,
                includeOutputs: true,
                inverse:        true,
                allowClose:     false
            }
        );

        // Load stage
        item.path = path;

        // Add to item cache
        stageItems[name] = item;

        // Return item
        return item;
    }

    function addStage(path, name, x, y)
    {
        // Create item for stage
        var item = stageComponent.createObject(
            stages,
            {
                pipeline: pipeline,
                x:        x || 100,
                y:        y || 100,
                name:     name || 'Stage'
            }
        );

        // Load stage
        item.path = path;

        // Add to item cache
        stageItems[name] = item;

        // Connect signal to remove the stage
        item.closed.connect(function()
        {
            removeStage(path, name);
        });

        // Return item
        return item;
    }

    function removeStage(path, name)
    {
        stageItems[name].destroy();

        delete stageItems[name];

        getStage(pipeline.path).removeStage(name);

        connectors.requestPaint();
    }

    function createStage(className, name)
    {
        var realName = getStage(pipeline.path).createStage(className, name);

        addStage(pipeline.path + '.' + realName, realName, 100, 100);
    }

    function computeLayout()
    {
        // [TODO]
    }

    function getStage(path)
    {
        var st = gloperate.canvas0.pipeline;

        var names = path.split('.');
        for (var i=0; i<names.length; i++)
        {
            var name = names[i];
            st = st[name];
        }

        return st;
    }

    function getSlotPos(path, type)
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
                    var pos = stageItem.getSlotPos(slotName, type);
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
                    var pos = inputsItem.getSlotPos(slotName, type);

                    if (pos)
                    {
                        return inputsItem.mapToItem(pipeline, pos.x, pos.y);
                    }

                    pos = outputsItem.getSlotPos(slotName, type);

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

    function elementEntered(path)
    {
        hoveredElement = path;

        connectors.requestPaint();
    }

    function elementLeft(path)
    {
        if (hoveredElement == path)
        {
            hoveredElement = '';
        }

        connectors.requestPaint();
    }

    function selectInput(path)
    {
        // If input slot is already connected, remove connection
        getStage(pipeline.path).removeConnection(path);

        // Connection created
        if (path != '' && selectedOutput != '')
        {
            // Create connection
            getStage(pipeline.path).createConnection(selectedOutput, path);

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

        connectors.requestPaint();
    }

    function selectOutput(path)
    {
        // Connection created
        if (path != '' && selectedInput != '')
        {
            // Create connection
            getStage(pipeline.path).createConnection(path, selectedInput);

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

        connectors.requestPaint();
    }
}
