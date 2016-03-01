
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  DockPanel
*
*  A panel that has a button to open and close it
*/
Panel
{
    id: panel

    property bool   open:       true
    property real   status:     1.0
    property string iconOpen:   ''
    property string iconClosed: ''
    property real   openTime:   600

    function openPanel()
    {
        icon.icon = iconOpen;
        status    = 1.0;
    }

    function hidePanel()
    {
        icon.icon = iconClosed;
        status    = 0.0;
    }

    Behavior on status
    {
        NumberAnimation
        {
            duration:    panel.openTime
            easing.type: Easing.OutQuad
        }
    }

    Icon
    {
        id: icon

        anchors.right:   parent.right
        anchors.top:     parent.top
        anchors.margins: Ui.style.panelPadding
        z:               100
        icon:            panel ? iconOpen : iconClosed

        onClicked:
        {
            panel.open = !panel.open;

            if (panel.open) panel.openPanel();
            else            panel.hidePanel();
        }
    }
}
