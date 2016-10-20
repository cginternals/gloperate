
import QtQuick 2.0
import QtQuick.Layouts 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0
import QmlToolbox.Ui 1.0
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

    // Viewer options
    GlOperatePipeline
    {
        id: gloperatePipeline

        onRootChanged:
        {
            optionAngle.update();
            optionRotate.update();
            optionTexture.update();
        }
    }

    ValueEdit
    {
        id: optionAngle

        pipelineInterface: gloperatePipeline
        path:              'pipeline.DemoPipeline.angle'

        anchors.left:    parent.left
        anchors.right:   parent.right
        anchors.bottom:  parent.bottom
        anchors.margins: Ui.style.paddingLarge
    }

    ValueEdit
    {
        id: optionRotate

        pipelineInterface: gloperatePipeline
        path:              'pipeline.DemoPipeline.rotate'

        anchors.left:    parent.left
        anchors.bottom:  optionAngle.top
        anchors.margins: Ui.style.paddingLarge
    }

    Label
    {
        text: 'Rotate'

        anchors.left:           optionRotate.right
        anchors.verticalCenter: optionRotate.verticalCenter
    }

    ValueEdit
    {
        id: optionTexture

        pipelineInterface: gloperatePipeline
        path:              'pipeline.DemoPipeline.texture'

        anchors.right:   parent.right
        anchors.bottom:  optionAngle.top
        anchors.margins: Ui.style.paddingLarge
    }

    Label
    {
        text: 'Texture:'

        anchors.right:          optionTexture.left
        anchors.rightMargin:    Ui.style.paddingMedium
        anchors.verticalCenter: optionTexture.verticalCenter
    }
}
