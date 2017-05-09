
import QtQuick 2.0
import QtQuick.Layouts 1.1

import QmlToolbox.Controls 1.0


Drawer
{
    id: drawer

    property string activePage:     ''
    property real   fullWidth:      parent.width
    property real   collapsedWidth: 0.3 * fullWidth
    property var    settingsObj:    null

    width:  activePage != '' ? fullWidth : collapsedWidth
    height: parent.height

    Behavior on width {
        PropertyAnimation { easing.type: Easing.InOutQuad }
    }

    onClosed:
    {
        drawer.activePage = '';
    }

    function setActivePage(page)
    {
        if (activePage !== page) activePage = page;
        else                     activePage = '';
    }

    ColumnLayout
    {
        id: content

        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        width:          drawer.collapsedWidth

        ToolButton
        {
            text: qsTr("Close")
            flat: true

            onClicked:
            {
                drawer.close()
            }
        }

        ToolButton
        {
            anchors.left:  parent.left
            anchors.right: parent.right

            flat: true
            text: qsTr("Load Pipeline")

            onClicked:
            {
                drawer.setActivePage('pipeline');
            }
        }

        ToolButton
        {
            anchors.left:  parent.left
            anchors.right: parent.right

            flat: true
            text: qsTr("Settings")

            onClicked:
            {
                drawer.setActivePage('settings');
            }
        }

        Item
        {
            Layout.fillHeight: true
        }
    }

    Pane
    {
        id: pipelinePage

        anchors.left:   content.right
        anchors.right:  parent.right
        anchors.top:    parent.top
        anchors.bottom: parent.bottom

        visible: drawer.activePage == 'pipeline'

        PipelinePage
        {
            anchors.fill: parent
        }
    }

    Pane
    {
        id: settingsPage

        anchors.left:   content.right
        anchors.right:  parent.right
        anchors.top:    parent.top
        anchors.bottom: parent.bottom

        visible:  drawer.activePage == 'settings'

        SettingsPage
        {
            anchors.fill: parent

            settings: drawer.settingsObj
        }
    }
}
