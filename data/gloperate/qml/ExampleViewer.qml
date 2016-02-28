
import QtQuick 2.0
import gloperate.rendering 1.0
import gloperate.ui 1.0


Page
{
    id: page

    RenderItem
    {
        id: render

        anchors.fill: parent
    }

    Row
    {
        anchors.left:    render.left
        anchors.top:     parent.top
        anchors.margins: Ui.style.pagePadding
        spacing:         Ui.style.pageSpacing

        Dropdown
        {
            text: 'DemoStage'
            icon: '0190-menu.png'

            items: [
                { name: 'painter',    text: 'Choose Painter', icon: '0092-tv.png' },
                { name: 'screenshot', text: 'Screenshot',     icon: '0040-file-picture.png' },
                { name: 'video',      text: 'Video',          icon: '0021-video-camera.png' }
            ];

            onItemClicked:
            {
                if (name === 'painter')
                {
                }
            }
        }
    }

    Row
    {
        anchors.right:   parent.right
        anchors.top:     parent.top
        anchors.margins: Ui.style.pagePadding
        spacing:         Ui.style.pageSpacing

        Dropdown
        {
            text: 'Settings'
            icon: '0149-cog.png'

            items: [
                { name: 'preferences', text: 'Preferences', icon: '0150-cogs.png' },
                { name: 'debug',       text: 'Debug',       icon: '0153-aid-kit.png' }
            ];

            onItemClicked:
            {
                if (name === 'debug')
                {
                    Ui.debugMode = !Ui.debugMode;
                }
            }
        }

        Dropdown
        {
            text:  'Theme'
            icon:  '0207-eye.png'
            items: getStyles();

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

            onItemClicked:
            {
                Ui.setStyle(name);
            }
        }
    }
}
