
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

    Dropdown
    {
        id: button1

        anchors.left:    render.left
        anchors.top:     parent.top
        anchors.margins: Ui.style.pagePadding

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

    Button
    {
        id: button2

        anchors.right:   parent.right
        anchors.top:     parent.top
        anchors.margins: Ui.style.pagePadding

        text: 'Debug'
        icon: '0153-aid-kit.png'

        onClicked:
        {
            Ui.debugMode = !Ui.debugMode;
        }
    }

    Button
    {
        id: button3

        anchors.right:   button2.left
        anchors.top:     parent.top
        anchors.margins: Ui.style.pagePadding

        text:    'Settings'
        icon:    '0149-cog.png'
        enabled: false
    }
}
