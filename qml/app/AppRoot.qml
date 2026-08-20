import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

FixedAspectRatioWindow {
    id: appRoot

    property string rootGitCommitId: ""
    property string rootProjectVersion: ""
    property int rootFontSize: 10

    width: 1280
    height: 720
    visible: true
    title: qsTr("Sagar Island")
    color: "royalblue"


    Column {
        anchors.centerIn: parent
        spacing: 20

        Label {
            text: "Navigation Channels"

            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter

            color: "white"
            font.pixelSize: rootFontSize
            font.bold: true
            font.italic: true
            font.family: "Book Antiqua"
        }

        Label {
            text: "Version: " + rootProjectVersion + " (" + rootGitCommitId + ")"

            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter

            color: "white"
            font.pixelSize: rootFontSize
            font.bold: true
            font.italic: true
            font.family: "Book Antiqua"
        }
    }
}
