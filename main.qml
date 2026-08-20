import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Sagar Island")
    color: "royalblue"

    Label {
        text: "Navigation Channels"

        anchors.centerIn: parent

        // Optional: Center the text formatting inside the label boundaries
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: "white"
        font.pixelSize: 24
        font.bold: true
        font.italic: true
        font.family: "Book Antiqua"
    }
}
