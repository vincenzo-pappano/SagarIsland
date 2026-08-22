import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15
import Qt.labs.settings 1.0

MapRefreshContainer {
    id: root

    //property var originalCenter: QtPositioning.coordinate(21.879612, 88.060324)
    property var originalCenter: QtPositioning.coordinate(21.917838, 88.090536)
    property int originalZoomLevel: 11
    property bool coordinatePickingEnabled: false
    property var pickedPath: []

    ListModel {
        id: pickedCoordinates
    }

    Shortcut {
        sequence: "Ctrl+p"
        context: Qt.WindowShortcut
        onActivated: {
            root.coordinatePickingEnabled = !root.coordinatePickingEnabled
            console.log("Map coordinate picking "
                        + (root.coordinatePickingEnabled ? "enabled" : "disabled"))
        }
    }

    Plugin {
        id: mapPlugin
        name: "esri"
    }

    Map {
        id: mainMap

        anchors.fill: parent

        center: root.originalCenter
        zoomLevel: root.originalZoomLevel

        plugin: mapPlugin

        Component.onCompleted: {
            for (var t = 0; t < supportedMapTypes.length; ++t) {
                if (supportedMapTypes[t].style === MapType.SatelliteMapDay) {
                    activeMapType = supportedMapTypes[t]
                    break
                }
            }
        } // Component

        MapPolyline {
            visible: root.pickedPath.length >= 2
            path: root.pickedPath
            line.width: 2
            line.color: "grey"
        }

        MapItemView {
            model: pickedCoordinates

            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(latitude, longitude)
                anchorPoint.x: 2
                anchorPoint.y: 2

                sourceItem: Rectangle {
                    width: 4
                    height: 4
                    radius: 2
                    color: "red"
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: root.coordinatePickingEnabled
            acceptedButtons: Qt.RightButton

            onClicked: {
                var coordinate = mainMap.toCoordinate(Qt.point(mouse.x, mouse.y), false)
                pickedCoordinates.append({
                    "latitude": coordinate.latitude,
                    "longitude": coordinate.longitude
                })
                root.pickedPath = root.pickedPath.concat([coordinate])
                console.log("Map coordinates: latitude=" + coordinate.latitude.toFixed(6)
                            + ", longitude=" + coordinate.longitude.toFixed(6))
            }
        }
    } // Map

    MapZoomControls {
        id: zoomControls
        // anchors.right: parent.right
        // anchors.rightMargin: 18
        // anchors.verticalCenter: parent.verticalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter
        zoomLevel: mainMap.zoomLevel
        minimumZoomLevel: mainMap.minimumZoomLevel
        maximumZoomLevel: mainMap.maximumZoomLevel

        onZoomInRequested: {
            mainMap.zoomLevel = Math.min(mainMap.maximumZoomLevel, mainMap.zoomLevel + 1)
        }

        onZoomOutRequested: {
            mainMap.zoomLevel = Math.max(mainMap.minimumZoomLevel, mainMap.zoomLevel - 1)
        }

        onResetRequested: {
            mainMap.center = root.originalCenter
            mainMap.zoomLevel = root.originalZoomLevel
        }

        // onZoomToAllRequested: {
        //     console.log(root.model)
        //     if (root.model)
        //         if (root.model.count > 0)
        //             mainMap.fitViewportToMapItems()
        //         else
        //             console.log("'count' is 0")
        //     else
        //         console.log("model is undefined")
        // }
    } // MapZoomControls}
}
