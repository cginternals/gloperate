
import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

import QmlToolbox.Base           1.0
import QmlToolbox.Controls       1.0
import QmlToolbox.Components     1.0
import QmlToolbox.PropertyEditor 1.0

import gloperate.rendering 1.0


ApplicationWindow
{
    id: window

    // Viewer hints
    property color backgroundColor:  'black'
    property bool  renderBackground: true

    // Stage
    property string stage: ''

    x:       settings.x
    y:       settings.y
    width:   settings.width
    height:  settings.height
    visible: false

    Shortcut
    {
        sequence: "CTRL+F6"
        onActivated: sidePanel.toggleVisible()
    }

    Shortcut
    {
        sequence: "CTRL+F7"
        onActivated: bottomPanel.toggleVisible()
    }

    Shortcut
    {
        sequence: "CTRL+F11"
        onActivated: togglePreviewMode();
    }

    Shortcut
    {
        sequence: "F11"
        onActivated: toggleFullScreenMode();
    }

    Shortcut
    {
        sequence: "ALT+RETURN"
        onActivated: toggleFullScreenMode();
    }

    Item
    {
        id: stateWrapper

        state: "normal"

        states:
        [
            State
            {
                name: "preview"

                StateChangeScript { script: sidePanel.setVisible(false) }
                StateChangeScript { script: bottomPanel.setVisible(false) }

                PropertyChanges
                {
                    target: window
                    header: null
                }

                PropertyChanges
                {
                    target:  mainMenu
                    visible: false
                }
            },

            State
            {
                name: "normal"

                StateChangeScript { script: sidePanel.setVisible(true) }
            }
        ]
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

    function togglePreviewMode()
    {
        stateWrapper.state = (stateWrapper.state == "normal") ? "preview" : "normal";
    }

    function toggleFullScreenMode()
    {
        fsStateWrapper.state = (fsStateWrapper.state == "windowedMode") ? "fullScreenMode" : "windowedMode";
    }

    header: ToolBar
    {
        id: toolBar

        RowLayout
        {
            anchors.fill: parent

            ToolButton
            {
                text: qsTr("Menu")
                onClicked: mainMenu.open()
            }

            Item
            {
                Layout.fillWidth: true
            }

            ToolButton
            {
                text: qsTr("Pipeline")
                onClicked: pipelineMenu.open()

                Menu
                {
                    id: pipelineMenu
                    y: toolBar.height

                    MenuItem
                    {
                        text: qsTr("Edit Pipeline")

                        onTriggered:
                        {
                            showEditor();
                        }
                    }
                }
            }

            ToolButton
            {
                text: qsTr("Tools")
                onClicked: toolsMenu.open()

                Menu
                {
                    id: toolsMenu
                    y: toolBar.height

                    MenuItem
                    {
                        text: qsTr("Take Screenshot")
                    }

                    MenuItem
                    {
                        text: qsTr("Record Video")
                    }
                }
            }

            ToolButton
            {
                text: qsTr("View")
                onClicked: viewMenu.open()

                Menu
                {
                    id: viewMenu
                    y: toolBar.height

                    MenuItem
                    {
                        text: sidePanel.isVisible() ? qsTr("Hide Side Panel") : qsTr("Show Side Panel")
                        onTriggered: sidePanel.toggleVisible()
                    }

                    MenuItem
                    {
                        text: bottomPanel.isVisible() ? qsTr("Hide Console") : qsTr("Show Console")
                        onTriggered: bottomPanel.toggleVisible()
                    }

                    MenuItem
                    {
                        text: sidePanel.isVisible() ? qsTr("Hide All") : qsTr("Show All")

                        onTriggered:
                        {
                            var visible = sidePanel.isVisible();
                            sidePanel  .setVisible(!visible);
                            bottomPanel.setVisible(!visible);
                        }
                    }
                }
            }

            ToolButton
            {
                text: (fsStateWrapper.state == "windowedMode") ? qsTr("Fullscreen") : qsTr("Windowed")
                onClicked: window.toggleFullScreenMode()
            }
        }
    }

    MainMenu
    {
        id: mainMenu

        settingsObj: settings

        onRenderStageSelected:
        {
            settings.stage = name;
            window.stage   = name;
            propertyEditor.update();
            internalPipelineView.update();
            externalPipelineView.update();
        }
    }

    // Container for the main view(s)
    Item
    {
        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.top:    parent.top
        anchors.bottom: bottomPanel.top

        // Main view
        Item
        {
            id: mainView

            anchors.fill: parent

            visible: true

            RenderItem
            {
                anchors.left:   sidePanel.position == 'left' ? sidePanel.right : parent.left
                anchors.right:  sidePanel.position == 'left' ? parent.right    : sidePanel.left
                anchors.top:    parent.top
                anchors.bottom: parent.bottom

                stage: window.stage

                onCanvasInitialized:
                {
                    gloperatePipeline.canvas = gloperate.canvas0;
                }
            }

            // Side Panel
            Panel
            {
                id: sidePanel

                position:    settings.panelPosition
                minimumSize: 240

                ScrollArea
                {
                    id: scrollArea

                    anchors.fill: parent

                    contentHeight: propertyEditor.height

                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds

                    PropertyEditor
                    {
                        id: propertyEditor

                        width: scrollArea.width

                        properties: gloperatePipeline
                        path:       'root'

                        Component.onCompleted:
                        {
                            propertyEditor.update();
                        }
                    }
                }
            }
        }

        // Pipeline view
        PipelineView
        {
            id: internalPipelineView

            anchors.fill: parent
            visible:      false

            properties:   gloperatePipeline
            path:         'root'
            toggleButton: 'Open in new window'

            onClosed:
            {
                hideEditor();
            }

            onToggled:
            {
                toggleEditor();
            }
        }
    }

    // Pipeline view in separate window
    Window
    {
        id: popoutWindow

        title: 'Pipeline Editor'

        minimumWidth:  750
        minimumHeight: 550

        PipelineView
        {
            id: externalPipelineView

            anchors.fill: parent

            properties:   gloperatePipeline
            path:         'root'
            toggleButton: 'Show inside main window'

            onClosed:
            {
                hideEditor();
            }

            onToggled:
            {
                toggleEditor();
            }
        }
    }

    // Toggles between internal and external pipeline editor
    function toggleEditor()
    {
        hideEditor();
        settings.editor = (settings.editor === "internal" ? "external" : "internal");
        showEditor();
    }

    // Shows the pipeline editor
    function showEditor()
    {
        if (settings.editor === "internal")
        {
            internalPipelineView.visible = true;
            mainView.visible             = false;
            internalPipelineView.load();
        } else {
            popoutWindow.showMaximized();
            popoutWindow.raise();
            externalPipelineView.load();
        }
    }

    // Hides the pipeline editor
    function hideEditor()
    {
        if (settings.editor === "internal")
        {
            mainView.visible             = true;
            internalPipelineView.visible = false;
        } else {
            popoutWindow.hide();
        }
    }

    // Bottom Panel
    Panel
    {
        id: bottomPanel

        position:    'bottom'
        minimumSize: 150
        visible:     false

        ScriptConsole
        {
            id: scriptConsole

            anchors.fill: parent

            keywords: ["console", "Math", "Date", "if", "for", "while", "function", "exit"]

            onSubmitted:
            {
                scriptConsole.output("> " + command + "\n");
                var res = executeScript(command);

                if (res != undefined)
                {
                    console.log(res);
                }
            }
        }
    }

    MessageForwarder
    {
        id: message_forwarder

        onMessageReceived:
        {
            // Put message on console log
            scriptConsole.output(message, type);
        }
    }

    GlOperatePipeline
    {
        id: gloperatePipeline

        onCanvasChanged:
        {
            propertyEditor.update();

            canvas.onStageInputChanged(function(slot, status)
            {
                gloperatePipeline.slotChanged('root', slot, status);
            });
        }
    }

    PreviewStage
    {
        id: previewStage
    }

    Settings
    {
        id: settings

        property int    x:             100
        property int    y:             100
        property int    width:         800
        property int    height:        600
        property int    logLevel:      3
        property bool   debugMode:     false
        property string panelPosition: 'left'
        property string editor:        'internal'
        property string stage:         ''
        property string pluginPaths:   ''

        onLogLevelChanged:
        {
            gloperate.system.setLogLevel(logLevel);
        }

        onDebugModeChanged:
        {
            Ui.debugMode = debugMode;
        }

        onPluginPathsChanged:
        {
            // Scan for plugins
            gloperate.components.setPluginPaths(settings.pluginPaths);
            gloperate.components.scanPlugins();
        }

        onEditorChanged:
        {
            gloperatePipeline.editor = settings.editor;
        }
    }

    Component.onCompleted:
    {
        if (gloperate.system.safeMode())
        {
            // Save default settings to config file
            settings.forceSave();
        } else {
            // Load settings
            settings.load();
        }

        // Scan for plugins
        gloperate.components.scanPlugins();

        // Add preview stage
        gloperatePipeline.clearInternalStages();
        gloperatePipeline.addInternalStage(previewStage.stageDefinition);

        // Set render stage
        window.stage = settings.stage;
        propertyEditor.update();

        // Show window
        window.visible = true;
    }

    Component.onDestruction:
    {
        settings.x      = x;
        settings.y      = y;
        settings.width  = width;
        settings.height = height;
    }
}
