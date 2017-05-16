
import QtQuick 2.4
import QtQuick.Layouts 1.1

import QmlToolbox.Base           1.0
import QmlToolbox.Controls       1.0
import QmlToolbox.Components     1.0
import QmlToolbox.PropertyEditor 1.0

import gloperate.rendering 1.0


ApplicationWindow
{
    id: window

    // Viewer hints
    property color backgroundColor:  'black'
    property bool  renderBackground: true

    // Stage
    property string stage: 'DemoPipeline'

    x:       100
    y:       100
    width:   800
    height:  600
    visible: true

    Shortcut
    {
        sequence: "F11"
        onActivated: toggleFullScreenMode();
    }

    Shortcut
    {
        sequence: "ALT+RETURN"
        onActivated: toggleFullScreenMode();
    }

    Item
    {
        id: fsStateWrapper

        state: "windowedMode"

        states:
        [
            State
            {
                name: "windowedMode"

                StateChangeScript { script: window.showNormal() }
            },

            State
            {
                name: "fullScreenMode"

                StateChangeScript { script: window.showFullScreen() }
            }
        ]
    }

    function toggleFullScreenMode()
    {
        fsStateWrapper.state = (fsStateWrapper.state == "windowedMode") ? "fullScreenMode" : "windowedMode";
    }

    // Main view
    RenderItem
    {
        anchors.fill: parent

        stage: window.stage
    }
}
