
import QtQuick 2.4
import QtQuick.Layouts 1.1

import QmlToolbox.Controls 1.0 as Controls
import QmlToolbox.Components 1.0 as Components
import QmlToolbox.PropertyEditor 1.0 as PropertyEditor
//import QmlToolbox.PipelineEditor 1.0

import com.cginternals.qmltoolbox 1.0

import gloperate.rendering 1.0


Controls.ApplicationWindow
{
    id: window

    x:       settings.x
    y:       settings.y
    width:   settings.width
    height:  settings.height
    visible: true

    // Viewer hints
    property color backgroundColor:  'black'
    property bool  renderBackground: true

    // Stage
    property string stage: 'DemoPipeline'

    // Shortcuts
    Controls.Shortcut 
    {
        sequence: "ESC"
        onActivated:
        {
            var visible = !toolBar.visible;

            leftPanelView.setPanelVisibility(visible);
            bottomPanelView.setPanelVisibility(visible);
            toolBar.visible = visible;
        }
    }

    Controls.Shortcut
    {
        sequence: "F11"
        onActivated: toggleFullScreenMode();
    }

    Controls.Shortcut
    {
        sequence: "ALT+RETURN"
        onActivated: toggleFullScreenMode();
    }

    GlOperatePipeline
    {
        id: gloperatePipeline

        onRootChanged:
        {
            propertyEditor.update();
        }
    }

    function toggleFullScreenMode()
    {
        fsStateWrapper.state = (fsStateWrapper.state == "windowedMode") ? "fullScreenMode" : "windowedMode";
    }

    Item
    {
        id: fsStateWrapper

        state: "windowedMode"

        states: 
        [
            State 
            {
                name: "windowedMode"

                StateChangeScript { script: window.showNormal() }
            },
            State 
            {
                name: "fullScreenMode"

                StateChangeScript { script: window.showFullScreen() }
            }
        ]
    }

    Components.Drawer 
    {
        id: drawer

        settingsContent: ColumnLayout 
        {
            anchors.fill: parent

            TestContent { }

            Item { Layout.fillHeight: true }
        }
    }

    header: Controls.ToolBar 
    {
        id: toolBar

        RowLayout 
        {
            anchors.fill: parent

            Controls.ToolButton 
            {
                text: qsTr("Menu")
                onClicked: drawer.open()
            }

            Item { Layout.fillWidth: true }

            Controls.ToolButton 
            {
                text: qsTr("Pipeline")
                onClicked: pipelineMenu.open()

                Controls.Menu {
                    id: pipelineMenu
                    y: toolBar.height

                    Controls.MenuItem { text: qsTr("Details") }
                    Controls.MenuItem { text: qsTr("Edit") }
                }
            }

            Controls.ToolButton 
            {
                text: qsTr("Tools")
                onClicked: toolsMenu.open()

                Controls.Menu 
                {
                    id: toolsMenu
                    y: toolBar.height

                    Controls.MenuItem { text: qsTr("Record") }
                    Controls.MenuItem { text: qsTr("Take Screenshot") }
                }
            }

            Controls.ToolButton 
            {
                text: qsTr("View")
                onClicked: viewMenu.open()

                Controls.Menu 
                {
                    id: viewMenu
                    y: toolBar.height

                    Controls.MenuItem 
                    { 
                        text: bottomPanelView.isPanelVisible() ? qsTr("Hide Console") : qsTr("Show Console")
                        onTriggered: bottomPanelView.togglePanel()
                    }

                    Controls.MenuItem 
                    {
                        text: leftPanelView.isPanelVisible() ? qsTr("Hide Side Panel") : qsTr("Show Side Panel")
                        onTriggered: leftPanelView.togglePanel()
                    }
                }
            }

            Controls.ToolButton
            {
                text: (fsStateWrapper.state == "windowedMode") ? qsTr("Fullscreen") : qsTr("Windowed")
                onClicked: window.toggleFullScreenMode()
            }
        }
    }

    Components.BottomPanelView 
    {
        id: bottomPanelView

        anchors.fill: parent

        Components.LeftPanelView 
        {
            id: leftPanelView

            anchors.fill: parent

            RenderItem
            {
                anchors.fill: parent

                stage: window.stage

                onCanvasInitialized:
                {
                    gloperatePipeline.root = gloperate.canvas0.pipeline;
                }
            }

            panel.minimumWidth: 240

            panelContent: Components.ScrollableFlickable 
            {
                anchors.fill: parent

                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds

                contentHeight: propertyEditor.height
                contentWidth: propertyEditor.width
            
                PropertyEditor.PropertyEditor 
                {
                    id: propertyEditor

                    pipelineInterface: gloperatePipeline
                    path: 'pipeline.DemoPipeline'

                    Component.onCompleted: propertyEditor.update()
                }

                verticalScrollbar: true
            }
        }

        panel.minimumHeight: 150

        panelContent: ColumnLayout 
        {
            anchors.fill: parent

            Components.Console 
            {
                id: console_view

                anchors.left: parent.left
                anchors.right: parent.right

                rightPadding: 0

                Layout.minimumHeight: 50
                Layout.fillHeight: true

                MessageForwarder 
                {
                    id: message_forwarder

                    onMessageReceived: 
                    {
                        var stringType;
                        if (type == MessageForwarder.Debug)
                            stringType = "Debug";
                        else if (type == MessageForwarder.Warning)
                            stringType = "Warning"; 
                        else if (type == MessageForwarder.Critical)
                            stringType = "Critical";
                        else if (type == MessageForwarder.Fatal)
                            stringType = "Fatal";

                        console_view.append(message, stringType);
                    }
                }
            }

            Components.CommandLine 
            {
                id: command_line

                anchors.left: parent.left
                anchors.right: parent.right

                autocompleteModel: ["console", "string", "for", "while"]

                onSubmitted: 
                {
                    console_view.append("> " + command + "\n", "Command");
                    var res = eval(command);

                    if (res != undefined)
                        console.log(res);
                }
            }
        }
    }

//  Labs.Settings
    QtObject
    {
        id: settings

        property int width: 800
        property int height: 600
        property int x
        property int y
    }

    Component.onDestruction: 
    {
        settings.x = x;
        settings.y = y;
        settings.width = width;
        settings.height = height;
    }
}
