import QtQuick 2.15

Item {
    id: root

    /*
     * Objects declared inside MapRefreshContainer
     * are automatically placed inside contentItem.
     */
    default property alias contentData: contentItem.data

    property int refreshNudge: 0

    onHeightChanged: {
        if (visible)
            mapRefreshTimer.restart()
    }

    Item {
        id: contentItem

        anchors.fill: parent
        anchors.rightMargin: root.refreshNudge
    }

    Timer {
        id: mapRefreshTimer

        interval: 0
        repeat: false

        onTriggered: {
            root.refreshNudge = 1
            mapRefreshResetTimer.restart()
        }
    }

    Timer {
        id: mapRefreshResetTimer

        interval: 0
        repeat: false

        onTriggered: {
            root.refreshNudge = 0
        }
    }
}