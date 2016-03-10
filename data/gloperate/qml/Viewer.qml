
import QtQuick 2.0
import gloperate.rendering 1.0
import gloperate.ui 1.0


Page
{
    id: page

    // Left panel
    DockPanel
    {
        id: panelLeft

        anchors.top:          page.top
        anchors.bottom:       panelBottom.top
        anchors.left:         page.left
        anchors.topMargin:    Ui.style.pagePadding
        anchors.bottomMargin: Ui.style.pagePadding
        anchors.leftMargin:   Ui.style.pagePadding * status + (24 - width) * (1.0 - status)
        width:                300

        iconClosed: '0323-circle-right.png'
        iconOpen:   '0325-circle-left.png'
    }

    // Bottom panel
    DockPanel
    {
        id: panelBottom

        anchors.left:         page.left
        anchors.right:        page.right
        anchors.bottom:       page.bottom
        anchors.leftMargin:   Ui.style.pagePadding
        anchors.rightMargin:  Ui.style.pagePadding
        anchors.bottomMargin: Ui.style.pagePadding * status + (36 - height) * (1.0 - status)
        height:               page.height / 3

        iconClosed: '0322-circle-up.png'
        iconOpen:   '0324-circle-down.png'

        TabBar
        {
            id: tabs

            anchors.top:     parent.top
            anchors.left:    parent.left
            anchors.margins: Ui.style.panelPadding

            items: [
                { name: 'output',    text: 'Output',    icon: '0035-file-text.png' },
                { name: 'scripting', text: 'Scripting', icon: '0086-keyboard.png' }
            ]

            selectedItem: 'output'
        }

        LogView
        {
            anchors.left:    parent.left
            anchors.right:   parent.right
            anchors.top:     tabs.bottom
            anchors.bottom:  parent.bottom
            anchors.margins: Ui.style.panelPadding

            visible: tabs.selectedItem == 'output'
        }

        ScriptConsole
        {
            anchors.left:    parent.left
            anchors.right:   parent.right
            anchors.top:     tabs.bottom
            anchors.bottom:  parent.bottom
            anchors.margins: Ui.style.panelPadding

            visible: tabs.selectedItem == 'scripting'
        }
    }

    // Main area
    Item
    {
        id: main

        anchors.left:    panelLeft.right
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

        items: [
          { name: 'stage', text: 'DemoStage', icon: '0190-menu.png', enabled: true,
            items: [
              { name: 'painter',    text: 'Choose Painter', icon: '0092-tv.png', enabled: false },
              { name: 'screenshot', text: 'Screenshot',     icon: '0040-file-picture.png', enabled: false },
              { name: 'video',      text: 'Video',          icon: '0021-video-camera.png', enabled: false }
            ] },
          { name: 'navigation', text: 'WorldInHand', icon: '0021-video-camera.png', enabled: true,
            items: [
              { name: 'worldinhand', text: 'WorldInHand', icon: '0021-video-camera.png', enabled: false },
              { name: 'orbiter',     text: 'Orbiter',     icon: '0021-video-camera.png', enabled: false },
              { name: 'flight',      text: 'Flight',      icon: '0021-video-camera.png', enabled: false }
            ] },
          { name: 'test', text: 'Test', icon: '0023-pacman.png', enabled: true }
        ];

        onItemClicked: // (menu, name)
        {
            if (menu == 'test') {
                welcome.openPanel();
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

        items: [
          { name: 'settings', text: 'Settings', icon: '0149-cog.png', enabled: true,
            items: [
              { name: 'preferences', text: 'Preferences', icon: '0150-cogs.png', enabled: true },
              { name: 'debug',       text: 'Debug',       icon: '0153-aid-kit.png' }
            ] },
          { name: 'theme', text: 'Theme', icon: '0207-eye.png', enabled: true,
            items: getStyles() }
        ];

        onItemClicked: // (menu, name)
        {
            if (menu == 'settings' && name == 'debug')
            {
                Ui.debugMode = !Ui.debugMode;
            }

            if (menu == 'settings' && name == 'preferences')
            {
                settings.openPanel();
            }

            if (menu == 'theme' && name != '')
            {
                Ui.setStyle(name);
            }
        }

        function getStyles()
        {
            var styles = [];

            for (var i=0; i<Ui.styles.length; i++)
            {
                styles.push({
                    name: Ui.styles[i],
                    text: Ui.styles[i],
                    icon: '0218-star-full.png'
                } );
            }

            return styles;
        }
    }

    // Welcome dialog
    WelcomeDialog
    {
        id: welcome

        anchors.fill:    main
        anchors.margins: Ui.style.dialogPadding

        visible: open
        status:  0.0
    }

    // Settings dialog
    Settings
    {
        id: settings

        anchors.fill:    main
        anchors.margins: Ui.style.dialogPadding

        visible: open
        status:  0.0
    }

    // Renderer
    RenderItem
    {
        id: render

        anchors.fill: main
        z:            -1
    }
}
