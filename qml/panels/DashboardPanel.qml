import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property string gitCommitId: ""
    property string projectVersion: ""
    property int labelFontSize: 10

    color: "royalblue"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Navigation Channels"
            color: "white"
            font.pixelSize: labelFontSize
            font.bold: true
            font.italic: true
            font.family: "Book Antiqua"
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Version: " + projectVersion + " (" + gitCommitId + ")"
            color: "white"
            font.pixelSize: labelFontSize
            font.bold: true
            font.italic: true
            font.family: "Book Antiqua"
        }
    }
}
