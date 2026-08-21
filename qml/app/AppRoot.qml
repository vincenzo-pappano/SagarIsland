import QtQuick 2.15
import QtQuick.Window 2.15
import "../panels"

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


    Row {
        anchors.fill: parent

        MapPanel {
            width: parent.width * 4 / 5
            height: parent.height
            labelFontSize: appRoot.rootFontSize
        }

        DashboardPanel {
            width: parent.width / 5
            height: parent.height
            gitCommitId: appRoot.rootGitCommitId
            projectVersion: appRoot.rootProjectVersion
            labelFontSize: appRoot.rootFontSize
        }
    }
}
