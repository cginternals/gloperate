
import QtQuick 2.0
import gloperate.rendering 1.0
import gloperate.ui 1.0


Page
{
    id: page

    // Renderer
    RenderItem
    {
        id: render

        anchors.fill: parent
    }

    // Top-left menu
    ButtonBar
    {
        id: menuLeft

        anchors.left:    render.left
        anchors.top:     parent.top
        anchors.margins: Ui.style.pagePadding

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
            ] }
        ];

        onItemClicked: // (menu, name)
        {
        }
    }

    // Top-right menu
    ButtonBar
    {
        id: menuRight

        anchors.right:   parent.right
        anchors.top:     parent.top
        anchors.margins: Ui.style.pagePadding

        items: [
          { name: 'settings', text: 'Settings', icon: '0149-cog.png', enabled: true,
            items: [
              { name: 'preferences', text: 'Preferences', icon: '0150-cogs.png', enabled: false },
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
}
