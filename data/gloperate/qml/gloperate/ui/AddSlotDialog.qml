
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

    function setChoices(choices)
    {
        type.model = choices;
    }

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

            model: []
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
