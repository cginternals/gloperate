
import QtQuick 2.0

import gloperate.rendering 1.0


/**
*  PreviewStage
*
*  Describes the state of a preview stage
*/
Item
{
    id: previewStage

    property var stageDefinition: internalStage ///< For access to stage definition

    property var previewComponent: null ///< Preview component on stage item in pipeline editor

    InternalStage
    {
        id: internalStage

        name:    'PreviewStage'
        path:    'root.PreviewStage'

        inputs: [
            {
                name:  'PreviewTexture',
                type:  'texture',
                value: null,
            }
        ]

        isVisibleInternal: true
        isVisibleExternal: false

        onConnectionCreated:
        {
            previewComponent.path = sourcePath + '.' + sourceSlot;
        }

        onConnectionRemoved:
        {
            previewComponent.path = '';
        }

        function onConfigure(stageItem)
        {
            previewComponent = stageItem.addComponent(newPreview, {});
        }
    }

    property Component newPreview : PreviewItem
    {
    }
}
