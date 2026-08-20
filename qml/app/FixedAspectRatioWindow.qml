

import QtQuick 2.15
import CustomWindow 1.0 as CustomWindowModule

CustomWindowModule.FixedAspectRatioWindow {
    id: window

    /*
     * The magic is this line:
     *
     * default property alias contentData: scaledContent.data
     *
     * It turns ScaledFixedAspectRatioWindow into a reusable container.
     * Anything placed inside it in AppRoot.qml is automatically inserted into scaledContent
     * and receives the aspect-ratio scaling.
     *
     */
    default property alias contentData: scaledContent.data

    property real baseWidth: 1920
    property real baseHeight: 1080

    readonly property real currentScale: Math.min(
                                             width / baseWidth,
                                             height / baseHeight
                                         )

    width: 1280
    height: 720

    minimumWidth: 800
    minimumHeight: 450

    aspectRatio: baseWidth / baseHeight

    Item {
        id: scaledContent

        width: window.baseWidth
        height: window.baseHeight

        x: (window.width - width * window.currentScale) / 2
        y: (window.height - height * window.currentScale) / 2

        transform: Scale {
            origin.x: 0
            origin.y: 0

            xScale: window.currentScale
            yScale: window.currentScale
        }
    }
}
