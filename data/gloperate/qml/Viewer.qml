
import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import gloperate.rendering 1.0
import gloperate.base 1.0
import gloperate.ui 1.0


Page
{
    id: page

    // Stage
    property string stage: 'DemoPipeline'
//  property string stage: 'DemoStage'

    // UI status
    property real uiStatus: 1.0
    readonly property bool uiEnabled: uiStatus > 0.0

    focus: true

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

    // Bottom icons
    Row
    {
        id: tabs

        anchors.bottom:  panelBottom.top
        anchors.left:    parent.left
        anchors.margins: Ui.style.panelPadding
        spacing:         Ui.style.pageSpacing

        visible: page.uiEnabled
        opacity: page.uiStatus

        property string selected: ''

        Button
        {
            property string title: 'log'

            icon:        '0035-file-text.png'
            highlighted: tabs.selected == title

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

        Button
        {
            property string title: 'script'

            icon:        '0086-keyboard.png'
            highlighted: tabs.selected == title

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
    Panel
    {
        id: panelBottom

        anchors.left:         page.left
        anchors.right:        page.right
        anchors.bottom:       page.bottom
        anchors.leftMargin:   Ui.style.pagePadding
        anchors.rightMargin:  Ui.style.pagePadding
        anchors.bottomMargin: status * (height + Ui.style.pagePadding) - height
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
            anchors.margins: Ui.style.panelPadding

            visible: tabs.selected == 'log'
        }

        ScriptConsole
        {
            anchors.fill:    parent
            anchors.margins: Ui.style.panelPadding

            visible: tabs.selected == 'script'
        }
    }

    // Main area
    Item
    {
        id: main

        anchors.left:    page.left
        anchors.right:   page.right
        anchors.top:     page.top
        anchors.bottom:  panelBottom.top
        anchors.margins: Ui.style.pagePadding
    }

    // Top-left menu
    ButtonBar
    {
        id: menuLeft

        anchors.left:    main.left
        anchors.top:     main.top
        anchors.margins: Ui.style.panelPadding

        visible: page.uiEnabled
        opacity: page.uiStatus

        items: [
          { name: 'pipeline', text: 'Demo', icon: '0190-menu.png', enabled: true,
            items: [
              { name: 'choose',     text: 'Choose Pipeline', icon: '0092-tv.png', enabled: false },
              { name: 'screenshot', text: 'Screenshot',      icon: '0040-file-picture.png', enabled: true },
              { name: 'video',      text: 'Video',           icon: '0021-video-camera.png', enabled: true },
              { name: 'edit'  ,     text: 'Edit Pipeline',   icon: '0387-share2.png', enabled: true }
            ]
          }
        ];

        onItemClicked: // (menu, name)
        {
            if (name == 'screenshot')
            {
                screenshot.visible = true;
            }

            else if (name == 'video')
            {
                video.visible = true;
                videoDialog.update();
            }

            else if (name == 'edit')
            {
                pipelineEditor.visible = !pipelineEditor.visible;
                pipelineEditor.load();
            }
        }
    }

    // Top-right menu
    ButtonBar
    {
        id: menuRight

        anchors.right:   main.right
        anchors.top:     main.top
        anchors.margins: Ui.style.pagePadding

        visible: page.uiEnabled
        opacity: page.uiStatus

        items: [
            { name: 'settings', text: 'Settings', icon: '0149-cog.png', enabled: true }
        ];

        onItemClicked: // (menu, name)
        {
            settings.visible = true;
        }
    }

    // Renderer
    RenderItem
    {
        id: render

        anchors.top:    main.top
        anchors.bottom: main.bottom
        anchors.left:   main.left
        anchors.right:  pipelineEditor.left

        z: -1

        stage: page.stage
    }

    // Pipeline editor
    PipelineEditor
    {
        id: pipelineEditor

        anchors.right: main.right
        anchors.top:   main.top
        width:         visible ? main.width * 0.75 : 0
        height:        main.height

        visible: false

        Behavior on width
        {
            NumberAnimation
            {
                easing.type: Easing.InOutQuad
                duration:    600
            }
        }
    }

    // Settings dialog
    ApplicationWindow
    {
        id: settings

        title:   "Settings"
        visible: false
        width:   800
        height:  600

        Settings
        {
            anchors.fill: parent
        }
    }

    // Screenshot window
    Window
    {
        id: screenshot

        property int margin: Ui.style.paddingMedium

        title:  "Screenshot"
        width:  screenshotItem.layout.implicitWidth + 20 * margin
        height: screenshotItem.layout.implicitHeight + 2 * margin

        Screenshot
        {
            id: screenshotItem

            margin: screenshot.margin
            anchors.fill: parent

            onClose: {
                screenshot.close();
            }
        }
    }

    // Video capture window
    Window
    {
        id: video

        property int margin: Ui.style.paddingMedium

        title:  "Video"
        width:  videoDialog.layout.implicitWidth + 20 * margin
        height: videoDialog.layout.implicitHeight + 2 * margin

        VideoDialog
        {
            id: videoDialog

            anchors.fill: parent
            margin:       screenshot.margin

            onClose: {
                video.close();
            }
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
                    anchors.margins: Ui.style.panelPadding
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
                    anchors.margins: Ui.style.panelPadding
                }
            }
        }
    }
}
