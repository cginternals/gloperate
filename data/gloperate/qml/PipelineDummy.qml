
import QtQuick 2.4


QtObject 
{
    id: pipeline

    property string name: "PipelineDummy"
    property QtObject stage: QtObject 
    {
        id: stage

        property var inputs: 
        [
            qsTr("Name of pony"),
            qsTr("Activate magic abilities"),
            qsTr("Length of pony"),
            qsTr("Color of pony"),
            qsTr("Name of its left toe")
        ]

        property var slots: 
        [
            {
                name:    qsTr('Name of pony'),
                type:    'string',
                value:   'Erhardt',
                options: {}
            },
            {
                name:    qsTr('Activate magic abilities'),
                type:    'bool',
                value:   true,
                options: {}
            },
            {
                name:    qsTr('Length of pony'),
                type:    'float',
                value:   1.3,
                options: {}
            },
            {
                name:    qsTr('Color of pony'),
                type:    'color',
                value:   '#000000',
                options: {}
            },
            {
                name:    qsTr('Name of its left toe'),
                type:    'string',
                value:   'Kurt',
                options: {}
            },
        ]

        function getSlot(slotName) 
        {
            for (var i = 0; i < stage.slots.length; i++) 
            {
                var slot = stage.slots[i];
                if (slot.name == slotName) 
                {
                    return slot;
                }
            }

            return {
                name: '',
                type: '',
                value: null,
                options: {}
            };
        }

        function setSlotValue(slotName, value) 
        {
            var slot = getSlot(slotName);

            slot.value = value;
        }
    }

    function getStage(path) 
    {
        return pipeline.stage;
    }

    function getSlot(path) 
    {
        var stage = pipeline.stage;

        var names = path.split('.');
        var slotName = names[names.length - 1];
        return stage.getSlot(slotName);
    }

    function setSlotValue(path, value) 
    {
        var stage = pipeline.stage;

        // Set value
        var names = path.split('.');
        var slotName = names[names.length - 1];

        stage.setSlotValue(slotName, value);
    }
}
