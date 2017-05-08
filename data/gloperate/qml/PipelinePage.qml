
import QtQuick 2.0
import QtQuick.Layouts 1.1

import QmlToolbox.Base 1.0
import QmlToolbox.Controls 1.0


Item
{
    ColumnLayout
    {
        anchors.top:   parent.top
        anchors.left:  parent.left
        anchors.right: parent.right

        Label
        {
            text: qsTr("Select Pipeline")
            font.pointSize: Ui.style.fontSizeLarge
        }
    }
}
