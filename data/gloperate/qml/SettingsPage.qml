
import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0

import gloperate.rendering 1.0


Item
{
    id: item

    anchors.fill: parent

    property var settings: null

    Connections
    {
        target: settings

        onPluginPathsChanged:
        {
            pluginPathEditor.update();
        }
    }

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
                    model: [ 'Critical', 'Error', 'Warning', 'Message', 'Debug', 'Debug (2)', 'Debug (3)', 'Debug (4)', 'Debug (5)' ]

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

                    text: 'Plugin Paths'
                }

                // Plugin Paths Editor
                Column
                {
                    id: pluginPathEditor

                    property var pluginPaths: []

                    spacing: Ui.style.paddingMedium

                    function update()
                    {
                        pluginPaths = gloperate.components.pluginPaths();
                    }

                    Repeater
                    {
                        model: pluginPathEditor.pluginPaths.length

                        Row
                        {
                            spacing: Ui.style.paddingMedium

                            Label
                            {                                
                                text: pluginPathEditor.pluginPaths[index]
                            }

                            ClickLabel
                            {
                                text: 'Remove'
                                Layout.alignment: Qt.AlignRight

                                onClicked:
                                {
                                    gloperate.components.removePluginPath(pluginPathEditor.pluginPaths[index]);
                                    item.settings.pluginPaths = gloperate.components.getPluginPaths();
                                }
                            }
                        }
                    }

                    ClickLabel
                    {
                        text: 'Add'

                        FileDialog
                        {
                            id: fileDialog

                            selectFolder: true
                            folder:       shortcuts.home

                            onAccepted:
                            {
                                var path = QmlUtils.urlToLocalFile(fileDialog.fileUrl);

                                gloperate.components.addPluginPath(path);
                                item.settings.pluginPaths = gloperate.components.getPluginPaths();
                            }
                        }

                        onClicked:
                        {
                            fileDialog.open();
                        }
                    }
                }
            }
        }
    }
}
