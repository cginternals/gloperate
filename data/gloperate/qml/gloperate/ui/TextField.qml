
import QtQuick 2.0
import QtQuick.Controls 1.1 as QtQuickControls
import gloperate.ui 1.0


/**
*  TextField
*
*  Single-line editable text
*/
Item
{
    id: textInput

    signal textEdited(string text)
    signal accepted()

    property string text:            ''
    property string placeholderText: ''

    implicitWidth:  input.implicitWidth
    implicitHeight: input.implicitHeight

    QtQuickControls.TextField
    {
        id: input

        width:           parent.width
        font.pixelSize:  Ui.style.ctrlTextSize
        text:            textInput.text
        placeholderText: textInput.placeholderText
        readOnly:        false

        onTextChanged:
        {
            if (text != textInput.text)
            {
                textEdited(text);
                textInput.text = text;
            }
        }

        onAccepted:
        {
            textInput.accepted();
        }
    }
}
