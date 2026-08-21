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