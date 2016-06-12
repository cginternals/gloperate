
import QtQuick 2.0
import QtQuick.Controls 1.1 as Controls
import QtQuick.Controls.Styles 1.1
import gloperate.base 1.0


/**
*  CheckBox
*
*  Check box wrapper
*/
Control
{
    id: item

    implicitWidth:  cb.implicitWidth
    implicitHeight: cb.implicitHeight

    property alias activeFocusOnPress:      cb.activeFocusOnPress
    property alias checked:                 cb.checked
    property alias checkedState:            cb.checkedState
    property alias exclusiveGroup:          cb.exclusiveGroup
    property alias hovered:                 cb.hovered
    property alias partiallyCheckedEnabled: cb.partiallyCheckedEnabled
    property alias pressed:                 cb.pressed
    property alias style:                   cb.style
    property alias text:                    cb.text

    Controls.CheckBox
    {
        id: cb

        style: CheckBoxStyle
        {
            label: Label
            {
                text: cb.text
            }
        }

        onClicked:
        {
            item.clicked();
        }
    }
}
