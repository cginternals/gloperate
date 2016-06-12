
import QtQuick 2.0
import QtQuick.Controls 1.1 as Controls
import gloperate.base 1.0


/**
*  Switch
*
*  A switch with a label
*/
Control
{
    id: item

    // Switch text
    property alias text: label.text

    // Switch options
    property alias activeFocusOnPress: button.activeFocusOnPress
    property alias checked:            button.checked
    property alias exclusiveGroup:     button.exclusiveGroup
    property alias style:              button.style

    implicitWidth:  row.implicitWidth  + 2 * Ui.style.ctrlPadding
    implicitHeight: row.implicitHeight + 2 * Ui.style.ctrlPadding

    Item
    {
        id: row

        property real spacing: Ui.style.ctrlSpacing

        anchors.fill:    parent
        anchors.margins: Ui.style.ctrlPadding
        implicitWidth:   label.implicitWidth + button.implicitWidth + spacing + 2 * anchors.margins
        implicitHeight:  Math.max(label.implicitHeight, button.implicitHeight) + 2 * anchors.margins

        Label
        {
            id: label

            anchors.verticalCenter: parent.verticalCenter

            enabled: item.enabled
        }

        Controls.Switch
        {
            id: button

            anchors.right:          parent.right
            anchors.verticalCenter: parent.verticalCenter

            onCheckedChanged:
            {
                item.clicked();
            }
        }
    }
}
