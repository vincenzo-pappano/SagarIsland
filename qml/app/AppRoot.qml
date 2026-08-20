import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

FixedAspectRatioWindow {
    id: appRoot

    property string rootGitCommitId: ""
    property int rootFontSize: 10

    width: 1280
    height: 720
    visible: true
    title: qsTr("Sagar Island")
    color: "royalblue"

    Label {
        text: "Navigation Channels (git commit ID: " + rootGitCommitId + ")"

        anchors.centerIn: parent

        // Optional: Center the text formatting inside the label boundaries
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: "white"
        font.pixelSize: rootFontSize
        font.bold: true
        font.italic: true
        font.family: "Book Antiqua"
    }
}
