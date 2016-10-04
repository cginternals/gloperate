
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Ui 1.0

import gloperate.rendering 1.0


Background
{
    id: page

    // Stage
    property string stage: 'DemoStage'
//  property string stage: 'DemoPipeline'

    // UI status
    property real uiStatus: 1.0

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

    renderBackground: false
    focus:            true

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
        }
    }

    // Top-right menu
    ButtonBar
    {
        id: menuRight

        anchors.right:   main.right
        anchors.top:     main.top
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

    // Renderer
    RenderItem
    {
        id: render

        anchors.fill: main
        z:            -1

        stage: page.stage
    }

    // Settings dialog
    Settings
    {
        id: settings
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
