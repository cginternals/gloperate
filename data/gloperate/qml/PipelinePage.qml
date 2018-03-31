
import QtQuick 2.0
import QtQuick.Layouts 1.1

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


ScrollArea
{
    id: item

    /// Called when a render stage has been selected
    signal renderStageSelected(string name)

    property var categories:      []      ///< List of categories
    property var stages:          {}      ///< List of stages, sorted by categories ( { 'cat1': [ { name: '', description: '', ... } ], 'cat2': [], ...})
    property var currentCategory: 'Demos' ///< The currently selected category
    property var currentStage:    ''      ///< The currently selected stage

    contentWidth:  layout.width
    contentHeight: layout.height

    ColumnLayout
    {
        id: layout

        spacing: Ui.style.paddingLarge

        Label
        {
            text: qsTr('Select Pipeline')
            font.pointSize: Ui.style.fontSizeLarge
        }

        RowLayout
        {
            spacing: Ui.style.paddingLarge

            ColumnLayout
            {
                anchors.top: parent.top

                Repeater
                {
                    model: item.categories.length

                    ClickLabel
                    {
                        id: label

                        text:     item.categories[index]
                        selected: item.currentCategory === text

                        onClicked:
                        {
                            item.currentCategory = text;
                        }
                    }
                }
            }

            ColumnLayout
            {
                anchors.top: parent.top

                Repeater
                {
                    model: item.stages === undefined ? 0 : item.stages[item.currentCategory].length

                    ClickLabel
                    {
                        id: label

                        text:     item.stages[item.currentCategory][index].name
                        selected: item.currentStage === text

                        onClicked:
                        {
                            item.currentStage = text;
                            item.renderStageSelected(text);
                        }
                    }
                }
            }
        }
    }

    onVisibleChanged:
    {
        if (visible) {
            updateComponents();
        }
    }

    function updateComponents()
    {
        var categories = [];
        var stages     = {};

        // Get list of components
        var components = gloperate.components.components();

        // Gather all components that have the 'RenderStage' tag and sort them by tags
        for (var i in components)
        {
            // Get component
            var component = components[i];
            var tags          = [];
            var isRenderStage = false;
    
            // Read tags from component
            var componentTags = component.tags.split(' ');
            for (var j in componentTags)
            {
                // Read tag
                var tag = componentTags[j];
                if (tag.length <= 0) continue;

                // Check RenderStage
                if (tag == 'RenderStage') {
                    isRenderStage = true;
                } else {
                    // Add tag to list
                    tags.push(tag);
                }
            }

            // Check if 'RenderStage' is set
            if (!isRenderStage) continue;

            // Sort component into tag lists and gather all tags
            for (var j in tags)
            {
                // Get tag
                var tag = tags[j];

                // Add tag to categories if it is not already contained
                if (categories.indexOf(tag) == -1)
                {
                    categories.push(tag);
                }

                // Put component into category
                if (!stages[tag]) stages[tag] = [];
                stages[tag].push(component);
            }
        }

        // Update categories and stages
        item.categories = categories;
        item.stages     = stages;
    }
}
