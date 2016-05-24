
import QtQuick 2.5
import gloperate.base 1.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4


/**
*  ProgressBar
*
*  Editable ProgressBar
*/
Controls.ProgressBar
{
    style: ProgressBarStyle {
        background: Rectangle {
            radius: Ui.style.panelBorderRadius
            color: Ui.style.backgroundColor
            border.color: Ui.style.getControlTextColor(item.enabled, item.highlighted, item.hovered, item.pressed)
            border.width: Ui.style.borderWidth
            implicitWidth: 200
            implicitHeight: 24
        }

        progress: Rectangle {
            radius: Ui.style.panelBorderRadius
            color: Ui.style.primaryColor
            border.color: Ui.style.getControlTextColor(item.enabled, item.highlighted, item.hovered, item.pressed)
        }
    }
}

