
import QtQuick 2.0
import QtQuick.Layouts 1.1

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


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
            }
        }
    }
}
