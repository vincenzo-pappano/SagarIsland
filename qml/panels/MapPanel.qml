import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property int labelFontSize: 10

    color: "steelblue"

    Label {
        anchors.centerIn: parent
        text: "Map"
        color: "white"
        font.pixelSize: labelFontSize
        font.bold: true
        font.family: "Book Antiqua"
    }
}
