
import QtQuick 2.0
import gloperate.base 1.0


/**
*  LineEdit
*
*  Single-line editable text
*/
Rectangle
{
    id: textInput

    signal textEdited(string text)
    signal accepted()

    property string text:            ''
    property string placeholderText: ''
    property bool   readOnly:        false
    property bool   selectByMouse:   true
    property color  backgroundColor: 'transparent'
    property color  textColor:       Ui.style.textColor

    implicitWidth:  input.implicitWidth
    implicitHeight: input.implicitHeight

    color: backgroundColor

    TextController
    {
        id: controller

        target: input
    }

    TextEdit
    {
        id: input

        width:           parent.width
        font.pixelSize:  Ui.style.ctrlTextSize
        text:            textInput.text
        readOnly:        textInput.readOnly
        selectByMouse:   textInput.selectByMouse
        color:           textInput.textColor

        onTextChanged:
        {
            if (text != textInput.text)
            {
                textEdited(text);
                textInput.text = text;
            }
        }

        Keys.onEnterPressed:
        {
            textInput.accepted();
        }

        /*
          Without this code, the editor respects the original distinction between Return
          and Enter: Return creates a new line, while Enter executes the code.
          However, this behaviour may be unfamiliar to most user. Uncomment this to
          enable the alternative mapping: Shift-Return creates a new line, Return and Enter
          execute the code.
        */
        Keys.onReturnPressed:
        {
            if ((event.modifiers & Qt.ShiftModifier) == 0) {
                textInput.accepted();
            } else {
                event.accepted = false;
            }
        }
    }
}
