
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
    id: item

    width:  childrenRect.x + childrenRect.width
    height: childrenRect.y + childrenRect.height

/*
    Connector
    {
        anchors.fill: parent

        x0: stage1.getOutputPos(0, stage1.x, stage1.y).x
        y0: stage1.getOutputPos(0, stage1.x, stage1.y).y
        x1: stage2.getInputPos (0, stage2.x, stage2.y).x
        y1: stage2.getInputPos (0, stage2.x, stage2.y).y
    }
*/

    /**
    *  Component that contains the template for a stage
    */
    property Component stageComponent: Stage
    {
    }

    /**
    *  Add new stage
    */
    function addStage(name, x, y)
    {
        return stageComponent.createObject(item, { x: x || 100, y: y || 100, name: name || 'Stage' });
    }

    /**
    *  Clear pipeline
    */
    function clear()
    {
        // Destroy all stages
        for (var i in item.children)
        {
            var stage = item.children[i];
            stage.destroy();
        }
    }

    /**
    *  Load pipeline from pipeline viewer
    *
    *  @param[in] path
    *    Path in the pipeline hierarchy (e.g., 'DemoPipeline.SubPipeline')
    */
    function load(path)
    {
        // Clear old pipeline
        clear();

        // Get pipeline container
        var pipelineContainer = gloperate.canvas0.pipeline;

        // Get pipeline
        var pipeline     = getStage(path);
        var pipelineDesc = pipeline.getDescription();

        // Add stages
        var x =  50;
        var y = 150;

        for (var i in pipelineDesc.stages)
        {
            var stageName = pipelineDesc.stages[i];

            // Get stage
            var stage = pipeline[stageName];
            var stageDesc = stage.getDescription();

            // Create stage in editor
            var item = addStage(stageName, x, y);
            item.load(path + '.' + stageName);

            x += item.width + 20;
        }

        // Do the layout
        computeLayout();
    }

    function computeLayout()
    {
        // [TODO]
    }

    function getStage(path)
    {
        var stage = gloperate.canvas0.pipeline;

        var names = path.split('.');
        for (var i=0; i<names.length; i++)
        {
            var name = names[i];
            stage = stage[name];
        }

        return stage;
    }
}
