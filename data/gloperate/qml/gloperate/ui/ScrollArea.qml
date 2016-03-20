
import QtQuick 2.0
import QtQuick.Controls 1.1


/**
*  ScrollArea
*
*  Provides an area with scrollable content if the content is larger than the
*  scroll area. You have to provide contentWidth and contentHeight with the
*  actual size of the child elements.
*/
ScrollView
{
    id: scrollArea

    default property alias data:  flick.flickableData

    property alias contentWidth:  flick.contentWidth
    property alias contentHeight: flick.contentHeight
    property alias contentX:      flick.contentX
    property alias contentY:      flick.contentY

    clip: true

    Flickable
    {
        id: flick

        anchors.fill: parent
    }
}
