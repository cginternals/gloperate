
import QtQuick 2.0
import gloperate.ui 1.0


/**
*  TextField
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

    Rectangle
    {
        anchors.fill: parent
        color:        textInput.backgroundColor
    }

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
        Keys.onReturnPressed:
        {
            textInput.accepted();
        }
*/
    }
}
