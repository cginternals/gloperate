
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.2


/**
*  AddSlotDialog
*
*  Dialog that allows the user to add new slots to a stage
*/
Dialog
{
    title:           'Add ' + slotType
    standardButtons: StandardButton.Ok | StandardButton.Cancel

    property string slotType: ''

    signal createSlot(string slotType, string type, string name)

    Column
    {
        width: parent.width

        spacing: 8

        Text
        {
            text: 'Slot Type:'
        }

        ComboBox
        {
            id: type

            width: parent.width

            model: [ 'float', 'int', 'vec2', 'vec3', 'vec4', 'ivec2', 'ivec3', 'ivec4', 'color', 'texture', 'fbo' ]
        }

        Text
        {
            text: 'Name:'
        }

        TextField
        {
            id: name

            width: parent.width

            text:            'slot'
            placeholderText: 'Enter name'
        }
    }

    onAccepted:
    {
        createSlot(slotType, type.currentText, name.text);
    }
}
