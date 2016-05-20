
import QtQuick 2.0
import QtQuick.Layouts 1.0
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

    property Component stageDelegate: null
    property string    targetStage:   ''

    implicitWidth:  row.width  + 2 * row.anchors.margins
    implicitHeight: row.height + 2 * row.anchors.margins

    Row
    {
        id: row

        anchors.left:    parent.left
        anchors.top:     parent.top
        anchors.margins: Ui.style.pipelinePadding

        spacing: 32

        Repeater
        {
            id: repeater

            delegate: stageDelegate
        }
    }

    onTargetStageChanged:
    {
        var lst = [];

        var stages = gloperate.pipeline.getStages(item.targetStage);
        for (var i=0; i<stages.length; i++) {
            var stage = stages[i];
            if (item.targetStage.length > 0) {
                stage = item.targetStage + '.' + stage;
            }

            lst.push(stage);
        }

        repeater.model = lst;
    }
}
