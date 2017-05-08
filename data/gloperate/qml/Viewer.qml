
import QtQuick 2.4
import QtQuick.Layouts 1.1

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
    property string stage: 'DemoPipeline'

    x:       settings.x
    y:       settings.y
    width:   settings.width
    height:  settings.height
    visible: true

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

            Item { Layout.fillWidth: true }

            ToolButton
            {
                text: qsTr("Pipeline")
                onClicked: pipelineMenu.open()

                Menu {
                    id: pipelineMenu
                    y: toolBar.height

                    MenuItem { text: qsTr("Edit Pipeline") }
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

                    MenuItem { text: qsTr("Take Screenshot") }
                    MenuItem { text: qsTr("Record Video") }
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
    }

    // Wrapper containing main page and side panel
    Item
    {
        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.top:    parent.top
        anchors.bottom: bottomPanel.top

        // Main page
        RenderItem
        {
            anchors.left:   sidePanel.position == 'left' ? sidePanel.right : parent.left
            anchors.right:  sidePanel.position == 'left' ? parent.right    : sidePanel.left
            anchors.top:    parent.top
            anchors.bottom: parent.bottom

            stage: window.stage

            onCanvasInitialized:
            {
                gloperatePipeline.root = gloperate.canvas0.pipeline;
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
                anchors.fill: parent

                contentHeight: propertyEditor.height
                contentWidth:  propertyEditor.width

                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds

                PropertyEditor
                {
                    id: propertyEditor

                    pipelineInterface: gloperatePipeline
                    path:              'pipeline.DemoPipeline'

                    Component.onCompleted:
                    {
                        propertyEditor.update()
                    }
                }
            }
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
                scriptConsole.output("> " + command + "\n", "Command");
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
            var stringType;

            if (type == MessageForwarder.Debug)
                stringType = "Debug";
            else if (type == MessageForwarder.Warning)
                stringType = "Warning";
            else if (type == MessageForwarder.Critical)
                stringType = "Critical";
            else if (type == MessageForwarder.Fatal)
                stringType = "Fatal";

            scriptConsole.output(message, stringType);
        }
    }

    GlOperatePipeline
    {
        id: gloperatePipeline

        onRootChanged:
        {
            propertyEditor.update();
        }
    }

    Settings
    {
        id: settings

        property int    x:             100
        property int    y:             100
        property int    width:         800
        property int    height:        600
        property bool   debugMode:     false
        property string panelPosition: 'left'

        onDebugModeChanged:
        {
            Ui.debugMode = debugMode;
        }
    }

    Component.onCompleted:
    {
        settings.load();
    }

    Component.onDestruction:
    {
        settings.x      = x;
        settings.y      = y;
        settings.width  = width;
        settings.height = height;
    }
}
