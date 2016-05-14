
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

    property string source: 'empty'

    implicitWidth:  row.width  + 2 * row.anchors.margins
    implicitHeight: row.height + 2 * row.anchors.margins

    Row
    {
        id: row

        anchors.left:    parent.left
        anchors.top:     parent.top
        anchors.margins: 16

        spacing: 32

        Repeater
        {
            id: repeater

            property var stages: []

            model: stages.length

            delegate: Stage
            {
                source: repeater.stages[index].name
            }
        }
    }

    onSourceChanged:
    {
        var lst = [];

        var stages = gloperate.pipeline.getStages(item.source);
        for (var i=0; i<stages.length; i++) {
            var stage = stages[i];
            if (item.source.length > 0) {
                stage = item.source.length + '.' + stage;
            }

            lst.push({
                name: stage
            });
        }

        repeater.stages = lst;
    }
}
