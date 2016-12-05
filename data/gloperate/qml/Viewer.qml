
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.Ui 1.0
import QmlToolbox.PipelineEditor 1.0
import QmlToolbox.PropertyEditor 1.0

import gloperate.rendering 1.0


Background
{
    id: page

    // Stage
    property string stage: 'DemoPipeline'

    // UI status
             property real uiStatus:  1.0
    readonly property bool uiEnabled: uiStatus > 0.0

    // Slowly fade out UI elements when deactivated
    Behavior on uiStatus
    {
        NumberAnimation
        {
            easing.type: Easing.InOutQuad
            duration:    1000
        }
    }

    // Hide all UI elements on Escape
    Keys.onPressed:
    {
        if (event.key == Qt.Key_Escape)
        {
            // Show/hide UI elements
            if (uiStatus > 0.0) {
                uiStatus = 0.0;
                tabs.selected = '';
            } else {
                uiStatus = 1.0;
            }
        }
    }

    // Disable background rendering, otherwise it would occlude the OpenGL widget
    renderBackground: false
    focus:            true

    // Renderer
    RenderItem
    {
        id: render

        anchors.fill: parent

        z: -1

        stage: page.stage

        onCanvasInitialized:
        {
            gloperatePipeline.root = gloperate.canvas0.pipeline;
        }
    }

    // Top-left menu
    ButtonBar
    {
        id: menuLeft

        anchors.left:    parent.left
        anchors.top:     parent.top
        anchors.margins: Ui.style.paddingMedium

        visible: page.uiEnabled
        opacity: page.uiStatus

        ButtonMenu
        {
            text: 'Demo'
            icon: '0190-menu.png'

            IconButton
            {
                text:    'Choose Pipeline'
                icon:    '0092-tv.png'
                enabled: false

                Layout.fillWidth: true

                onClicked:
                {
                }
            }

            IconButton
            {
                text: 'Screenshot'
                icon: '0040-file-picture.png'

                Layout.fillWidth: true

                onClicked:
                {
                    screenshot.visible = true;
                }
            }

            IconButton
            {
                text: 'Video'
                icon: '0021-video-camera.png'

                Layout.fillWidth: true

                onClicked:
                {
                    video.visible = true;
                    video.update();
                }
            }

            IconButton
            {
                text: 'Edit Pipeline'
                icon: '0387-share2.png'

                Layout.fillWidth: true

                onClicked:
                {
                    var name = gloperatePipeline.getStage('pipeline').stages[0];

                    pipelineEditor.load('pipeline.' + name);
                    pipelineEditorFrame.visible = !pipelineEditorFrame.visible;
                }
            }

            IconButton
            {
                text: 'Save Pipeline'
                icon: '0040-file-picture.png'

                Layout.fillWidth: true

                onClicked:
                {
                    pipelineSave.visible = true;
                }
            }
        }

        IconButton
        {
            text:    'Rec'
            icon:    '0021-video-camera.png'
            enabled: video.settingsApplied

            onClicked:
            {
                gloperate.canvas0.toggleVideoExport();
            }
        }
    }

    // Top-right menu
    ButtonBar
    {
        id: menuRight

        anchors.right:   parent.right
        anchors.top:     parent.top
        anchors.margins: Ui.style.paddingMedium

        visible: page.uiEnabled
        opacity: page.uiStatus

        IconButton
        {
            text: 'Settings'
            icon: '0149-cog.png'

            onClicked:
            {
                settings.visible = true;
            }
        }
    }

    // Bottom icons
    Row
    {
        id: tabs

        anchors.bottom:  panelBottom.top
        anchors.left:    parent.left
        anchors.margins: Ui.style.paddingSmall
        spacing:         Ui.style.spacingSmall

        visible: page.uiEnabled
        opacity: page.uiStatus

        property string selected: ''

        IconButton
        {
            property string title: 'log'

            icon:     '0035-file-text.png'
            selected: tabs.selected == title

            onClicked:
            {
                if (tabs.selected != title) tabs.selected = title;
                else                        tabs.selected = '';
            }

            onRightClicked:
            {
                logWindow.createObject(page, {});
            }
        }

        IconButton
        {
            property string title: 'script'

            icon:     '0086-keyboard.png'
            selected: tabs.selected == title

            onClicked:
            {
                if (tabs.selected != title) tabs.selected = title;
                else                        tabs.selected = '';
            }

            onRightClicked:
            {
                scriptWindow.createObject(page, {});
            }
        }
    }

    // Bottom panel
    Frame
    {
        id: panelBottom

        anchors.left:         page.left
        anchors.right:        page.right
        anchors.bottom:       page.bottom
        anchors.leftMargin:   Ui.style.paddingMedium
        anchors.rightMargin:  Ui.style.paddingMedium
        anchors.bottomMargin: status * (height + Ui.style.paddingMedium) - height
        height:               page.height / 3

        visible: page.uiEnabled
        opacity: page.uiStatus

        property real status: (tabs.selected != '') ? 1.0 : 0.0

        Behavior on status
        {
            NumberAnimation
            {
                easing.type: Easing.InOutQuad
                duration:    600
            }
        }

        LogView
        {
            anchors.fill:    parent
            anchors.margins: Ui.style.paddingMedium

            visible: tabs.selected == 'log'
        }

        ScriptConsole
        {
            anchors.fill:    parent
            anchors.margins: Ui.style.paddingMedium

            visible: tabs.selected == 'script'
        }
    }

    // Left panel
    Frame
    {
        id: panelLeft

        anchors.left:    parent.left
        anchors.top:     menuLeft.bottom
        anchors.bottom:  tabs.top
        anchors.margins: Ui.style.paddingMedium

        implicitWidth: peCol.implicitWidth + 2 * peCol.anchors.margins

        visible: page.uiEnabled
        opacity: 0.8 * page.uiStatus

        ScrollArea
        {
            anchors.fill: parent

            contentHeight: peCol.height + Ui.style.paddingMedium

            Column
            {
                id: peCol

                anchors.left:    parent.left
                anchors.right:   parent.right
                anchors.top:     parent.top
                anchors.margins: Ui.style.paddingMedium

                spacing: Ui.style.spacingMedium

                PropertyEditor
                {
                    id: propertyEditor

                    pipelineInterface: gloperatePipeline
                    path:              'pipeline.DemoPipeline'
                }

                Button
                {
                    text: 'Update'

                    onClicked:
                    {
                        propertyEditor.update();
                    }
                }
            }
        }
    }

    // Pipeline editor
    GlOperatePipeline
    {
        id: gloperatePipeline

        onRootChanged:
        {
            propertyEditor.update();
        }
    }

    Frame
    {
        id: pipelineEditorFrame

        anchors.fill: parent

        visible: false

        PipelineEditor
        {
            id: pipelineEditor

            anchors.fill: parent

            pipelineInterface: gloperatePipeline
        }

        Button
        {
            anchors.top:     parent.top
            anchors.left:    parent.left
            anchors.margins: Ui.style.paddingMedium

            text: 'Close'

            onClicked:
            {
                pipelineEditorFrame.visible = false;
            }
        }

        TextureRenderItem
        {
            x:      350
            y:      350
            width:  200
            height: 200

            path: 'pipeline.DemoPipeline.ColorizeStage.colorTextureOut'

            Drag.active: mouseArea.drag.active

            MouseArea
            {
                id: mouseArea

                anchors.fill: parent

                acceptedButtons: Qt.LeftButton
                drag.target:     parent
            }
        }
    }

    // Settings dialog
    Settings
    {
        id: settings
    }

    // Screenshot dialog
    ScreenshotDialog
    {
        id: screenshot
    }

    // Pipeline saving dialog
    PipelineSaveDialog
    {
        id: pipelineSave
    }

    // Video capture dialog
    VideoDialog
    {
        id: video

        property bool settingsApplied: false

        onClosed:
        {
            settingsApplied = true;
        }
    }

    // Log window
    Component
    {
        id: logWindow

        ApplicationWindow
        {

            title:   "Log"
            visible: true
            width:   600
            height:  800

            Background
            {
                anchors.fill: parent

                LogView
                {
                    anchors.fill:    parent
                    anchors.margins: Ui.style.paddingMedium
                }
            }
        }
    }

    // Scripting console window
    Component
    {
        id: scriptWindow

        ApplicationWindow
        {
            title:   "Scripting console"
            visible: true
            width:   600
            height:  800

            Background
            {
                anchors.fill: parent

                ScriptConsole
                {
                    anchors.fill:    parent
                    anchors.margins: Ui.style.paddingMedium
                }
            }
        }
    }
}
