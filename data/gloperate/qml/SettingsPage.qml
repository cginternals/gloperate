
import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0

import gloperate.rendering 1.0

Item
{
    anchors.fill: parent

    property var settings: null

    ScrollArea
    {
        anchors.fill: parent

        contentHeight: pane.height
        contentWidth:  pane.width

        Pane
        {
            id: pane

            implicitWidth:  grid.implicitWidth  + 2 * padding
            implicitHeight: grid.implicitHeight + 2 * padding

            GridLayout
            {
                id: grid

                anchors.fill: parent

                columns:       2
                columnSpacing: 16
                rowSpacing:    8

                Label
                {
                    text: qsTr('Settings')
                    font.pointSize: Ui.style.fontSizeLarge

                    Layout.columnSpan: 2
                }

                Item
                {
                    Layout.columnSpan: 2
                    Layout.fillHeight: true
                }

                Label
                {
                    Layout.alignment: Qt.AlignRight

                    text: 'Log Level'
                }

                ComboBox
                {
                    model: [ 'Critical', 'Error', 'Warning', 'Message', 'Debug', 'Debug (2)', 'Debug (3)', 'Debug (4)' ]

                    currentIndex: settings.logLevel

                    onActivated:
                    {
                        settings.logLevel = index;
                    }
                }

                Label
                {
                    Layout.alignment: Qt.AlignRight

                    text: 'Enable Debug Mode'
                }

                Switch
                {
                    text: ''
                    checked: settings.debugMode

                    onClicked:
                    {
                        settings.debugMode = !settings.debugMode;
                    }
                }

                Label
                {
                    Layout.alignment: Qt.AlignRight

                    text: 'Panel Position'
                }

                ComboBox
                {
                    model: [ 'left', 'right' ]

                    currentIndex: model.indexOf(settings.panelPosition)

                    onActivated:
                    {
                        var position = model[index];
                        settings.panelPosition = position;
                    }
                }

                Label
                {
                    Layout.alignment: Qt.AlignRight | Qt.AlignTop

                    text: 'Add Plugin Path'
                }

                // Plugin Paths Editor
                Column
                {
                    id: pluginPathEditor

                    property var pluginPaths: []

                    function updatePluginPaths() {
                        settings.pluginPaths = gloperate.components.getPluginPaths()
                        pluginPathEditor.pluginPaths = settings.pluginPaths.split(';')
                        gloperate.components.scanPlugins();
                        if(pluginPathEditor.pluginPaths == "") {pluginPathEditor.pluginPaths = []}

                    }

                    Repeater
                    {
                        model: pluginPathEditor.pluginPaths.length

                        Row
                        {
                            Label
                            {                                
                                text: pluginPathEditor.pluginPaths[index]
                            }

                            ClickLabel
                            {
                                text: " delete"
                                Layout.alignment: Qt.AlignRight

                                onClicked:
                                {
                                    gloperate.components.removePluginPath(pluginPathEditor.pluginPaths[index])
                                    pluginPathEditor.updatePluginPaths()
                                }
                            }
                        }
                    }

                    ClickLabel
                    {
                        text:   "Select Path"

                        FileDialog {
                            id: fileDialog
                            
                            selectFolder: true

                            folder: shortcuts.home
                            onAccepted: {
                                var path = fileDialog.fileUrl.toString();

                                // remove prefixed "file:///"
                                path = path.replace(/^(file:\/{3})/,"");

                                gloperate.components.addPluginPath(path)
                                pluginPathEditor.updatePluginPaths()
                            }
                        }

                        onClicked:
                        {
                            fileDialog.open()
                        }
                    }
                    
                    Component.onCompleted:
                    {
                        pluginPathEditor.updatePluginPaths()
                    }
                }

            }
        }
    }
}
