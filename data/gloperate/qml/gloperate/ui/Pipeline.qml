
import QtQuick 2.0

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

    property var stageItems: null ///< Item cache

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

        // Get pipeline
        var pl     = getStage(pipeline.path);
        var plDesc = pl.getDescription();

        // Add stages
        var x =  50;
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

    function addStage(path, name, x, y)
    {
        // Create item for stage
        var item = stageComponent.createObject(
            stages,
            {
                pipeline: pipeline,
                x: x || 100,
                y: y || 100,
                name: name || 'Stage'
            }
        );

        item.path = path;

        // Add to item cache
        stageItems[name] = item;

        // Return item
        return item;
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

                // [TODO]
            }
        }

        // Error in calling this function
        return null;
    }
}
