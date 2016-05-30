
import QtQuick 2.0
import gloperate.base 1.0
import QtQuick.Controls 1.2 as Controls
import QtQuick.Controls.Styles 1.4


/**
*  TextField
*
*  Single-line editable text
*/
Controls.TextField
{
    font.pixelSize:  Ui.style.ctrlTextSize

    style: TextFieldStyle {
        background: Rectangle {
            radius: Ui.style.panelBorderRadius
            color: Ui.style.fieldColor
            border.color: Ui.style.getControlTextColor(item.enabled, item.highlighted, item.hovered, item.pressed)
        }
    }
}
