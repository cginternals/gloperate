
import QtQuick 2.0
import GLOperate.Ui 1.0


/**
*  Label
*
*  Displays a text with default style
*/
Text
{
    id: label

    color:          Style.labelColor
    font.pixelSize: Style.labelFontSize
    wrapMode:       Text.Wrap

    text: 'Something'
}
