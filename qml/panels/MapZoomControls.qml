import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    // width: 56
    // height: 190
    width: 144
    height: 56
    radius: 4

    color: "#252a31"
    border.color: "#555c66"
    border.width: 1
    opacity: 0.95

    property real zoomLevel: 15
    property real minimumZoomLevel: 3
    property real maximumZoomLevel: 20

    signal zoomInRequested()
    signal zoomOutRequested()
    signal resetRequested()

    // Column
    Row {
        id: containerId
        anchors.fill: parent
        anchors.margins: 6
        spacing: 6

        ListModel {
            id: buttonModelId

            ListElement {
                label: "+"
                fontSize: 22
                tooltip: "Zoom in"
                isEnabled: function() { return root.zoomLevel < root.maximumZoomLevel }
                action: function() { root.zoomInRequested() }
            }
            ListElement {
                label: "-"
                fontSize: 22
                tooltip: "Zoom out"
                isEnabled: function() { return root.zoomLevel > root.minimumZoomLevel }
                action: function() { root.zoomOutRequested() }
            }
            ListElement {
                label: "⌂"
                fontSize: 24
                tooltip: "Reset to initial"
                isEnabled: function() { return true }
                action: function() { root.resetRequested() }
            }
        }
        Repeater {
            model: buttonModelId

            delegate: Button {
                width: 40
                height: parent.height

                text: model.label
                font.pixelSize: model.fontSize

                enabled: model.isEnabled()

                ToolTip.visible: hovered
                ToolTip.text: model.tooltip

                onClicked: model.action()
            }
        }
    }
}