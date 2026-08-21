import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15
import Qt.labs.settings 1.0

MapRefreshContainer {
    id: rootId

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

        center: rootId.originalCenter
        zoomLevel: rootId.originalZoomLevel

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
}
