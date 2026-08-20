#include "FixedAspectRatioWindow.h"

#include <QMargins>
#include <QDebug>
#include <QtGlobal>

#include <algorithm>
#include <cmath>

#ifdef Q_OS_WIN
#include <QtCore/qt_windows.h>
#endif

FixedAspectRatioWindow::FixedAspectRatioWindow(QWindow *parent)
    : QQuickWindow(parent)
{
}

qreal FixedAspectRatioWindow::aspectRatio() const
{
    return m_aspectRatio;
}

void FixedAspectRatioWindow::setAspectRatio(qreal ratio)
{
    constexpr qreal minimumSupportedRatio = 0.01;
    constexpr qreal maximumSupportedRatio = 100.0;

    if (!std::isfinite(ratio)
        || ratio < minimumSupportedRatio
        || ratio > maximumSupportedRatio) {
        qWarning() << "Ignoring invalid aspect ratio" << ratio
                   << "(expected a finite value from"
                   << minimumSupportedRatio << "to"
                   << maximumSupportedRatio << ')';
        return;
    }

    if (qFuzzyCompare(m_aspectRatio, ratio))
        return;

    m_aspectRatio = ratio;

    emit aspectRatioChanged();
}

bool FixedAspectRatioWindow::nativeEvent(
    const QByteArray &eventType,
    void *message,
    long *result)
{
#ifdef Q_OS_WIN

    MSG *msg = static_cast<MSG *>(message);

    /*
     * Interactive window resizing has started.
     *
     * Capture the original window center once. This prevents the center
     * from being recalculated from an already modified RECT on every
     * WM_SIZING event.
     */
    if (msg->message == WM_ENTERSIZEMOVE) {
        RECT windowRect;

        if (GetWindowRect(msg->hwnd, &windowRect)) {
            m_resizeCenterX =
                (windowRect.left + windowRect.right) / 2;

            m_resizeCenterY =
                (windowRect.top + windowRect.bottom) / 2;

            m_resizeCenterValid = true;
        }

        return QQuickWindow::nativeEvent(
            eventType,
            message,
            result);
    }

    /*
     * Interactive resizing has finished.
     */
    if (msg->message == WM_EXITSIZEMOVE) {
        m_resizeCenterValid = false;

        return QQuickWindow::nativeEvent(
            eventType,
            message,
            result);
    }

    if (msg->message == WM_SIZING) {
        RECT *rect =
            reinterpret_cast<RECT *>(msg->lParam);

        if (!rect)
            return false;

        /*
         * WM_SIZING gives us the complete native window rectangle,
         * including the title bar and borders.
         *
         * QQuickWindow::width and height represent the client area.
         */
        const qreal nativeScale =
            std::max(devicePixelRatio(), qreal(1.0));

        RECT currentWindowRect = {};
        RECT currentClientRect = {};

        int frameWidth = 0;
        int frameHeight = 0;

        if (GetWindowRect(msg->hwnd, &currentWindowRect)
            && GetClientRect(msg->hwnd, &currentClientRect)) {
            frameWidth =
                (currentWindowRect.right - currentWindowRect.left)
                - (currentClientRect.right - currentClientRect.left);

            frameHeight =
                (currentWindowRect.bottom - currentWindowRect.top)
                - (currentClientRect.bottom - currentClientRect.top);
        } else {
            const QMargins margins = frameMargins();

            frameWidth =
                qRound((margins.left() + margins.right()) * nativeScale);

            frameHeight =
                qRound((margins.top() + margins.bottom()) * nativeScale);
        }

        int clientWidth =
            rect->right - rect->left - frameWidth;

        int clientHeight =
            rect->bottom - rect->top - frameHeight;

        clientWidth = std::max(clientWidth, 1);
        clientHeight = std::max(clientHeight, 1);

        /*
         * If WM_ENTERSIZEMOVE was not received for some reason,
         * initialize the resize center from the current window position.
         */
        if (!m_resizeCenterValid) {
            RECT windowRect;

            if (GetWindowRect(msg->hwnd, &windowRect)) {
                m_resizeCenterX =
                    (windowRect.left + windowRect.right) / 2;

                m_resizeCenterY =
                    (windowRect.top + windowRect.bottom) / 2;
            } else {
                m_resizeCenterX =
                    (rect->left + rect->right) / 2;

                m_resizeCenterY =
                    (rect->top + rect->bottom) / 2;
            }

            m_resizeCenterValid = true;
        }

        /*
         * Calculate the smallest client dimensions that satisfy:
         *
         *     1. QML minimumWidth
         *     2. QML minimumHeight
         *     3. The requested aspect ratio
         */
        const int requestedMinimumWidth =
            std::max(qRound(minimumWidth() * nativeScale), 1);

        const int requestedMinimumHeight =
            std::max(qRound(minimumHeight() * nativeScale), 1);

        int minimumClientWidth =
            std::max(
                requestedMinimumWidth,
                static_cast<int>(
                    std::ceil(
                        requestedMinimumHeight *
                        m_aspectRatio)));

        int minimumClientHeight =
            std::max(
                requestedMinimumHeight,
                static_cast<int>(
                    std::ceil(
                        minimumClientWidth /
                        m_aspectRatio)));

        /*
         * Account for a possible one-pixel rounding difference.
         */
        minimumClientWidth =
            std::max(
                minimumClientWidth,
                static_cast<int>(
                    std::ceil(
                        minimumClientHeight *
                        m_aspectRatio)));

        bool widthControlsSize = true;

        switch (msg->wParam) {
        case WMSZ_LEFT:
        case WMSZ_RIGHT:
            /*
             * A horizontal edge is being dragged.
             * Width determines height.
             */
            widthControlsSize = true;
            break;

        case WMSZ_TOP:
        case WMSZ_BOTTOM:
            /*
             * A vertical edge is being dragged.
             * Height determines width.
             */
            widthControlsSize = false;
            break;

        default: {
            /*
             * A corner is being dragged.
             *
             * Use whichever dimension changed proportionally more as
             * the controlling dimension.
             */
            const qreal currentWidth =
                std::max(width() * nativeScale, 1.0);

            const qreal currentHeight =
                std::max(height() * nativeScale, 1.0);

            const qreal widthChange =
                std::abs(clientWidth - currentWidth) /
                currentWidth;

            const qreal heightChange =
                std::abs(clientHeight - currentHeight) /
                currentHeight;

            widthControlsSize =
                widthChange >= heightChange;

            break;
        }
        }

        /*
         * Calculate the ratio-corrected client dimensions while also
         * enforcing the QML minimum size.
         */
        if (widthControlsSize) {
            clientWidth =
                std::max(
                    clientWidth,
                    minimumClientWidth);

            clientHeight =
                static_cast<int>(
                    std::lround(
                        clientWidth /
                        m_aspectRatio));

            if (clientHeight < minimumClientHeight) {
                clientHeight = minimumClientHeight;

                clientWidth =
                    static_cast<int>(
                        std::lround(
                            clientHeight *
                            m_aspectRatio));
            }
        } else {
            clientHeight =
                std::max(
                    clientHeight,
                    minimumClientHeight);

            clientWidth =
                static_cast<int>(
                    std::lround(
                        clientHeight *
                        m_aspectRatio));

            if (clientWidth < minimumClientWidth) {
                clientWidth = minimumClientWidth;

                clientHeight =
                    static_cast<int>(
                        std::lround(
                            clientWidth /
                            m_aspectRatio));
            }
        }

        const int targetWindowWidth =
            clientWidth + frameWidth;

        const int targetWindowHeight =
            clientHeight + frameHeight;

        /*
         * Update the native rectangle.
         *
         * For side resizing, the opposite horizontal edge remains fixed
         * and the original vertical center remains fixed.
         *
         * This prevents the window from drifting vertically after the
         * minimum width has been reached.
         */
        switch (msg->wParam) {
        case WMSZ_LEFT:
            /*
             * The user is dragging the left edge.
             * Keep the right edge fixed.
             */
            rect->left =
                rect->right - targetWindowWidth;

            rect->top =
                m_resizeCenterY -
                targetWindowHeight / 2;

            rect->bottom =
                rect->top + targetWindowHeight;
            break;

        case WMSZ_RIGHT:
            /*
             * The user is dragging the right edge.
             * Keep the left edge fixed.
             */
            rect->right =
                rect->left + targetWindowWidth;

            rect->top =
                m_resizeCenterY -
                targetWindowHeight / 2;

            rect->bottom =
                rect->top + targetWindowHeight;
            break;

        case WMSZ_TOP:
            /*
             * Keep the bottom edge fixed.
             * Keep the original horizontal center fixed.
             */
            rect->top =
                rect->bottom - targetWindowHeight;

            rect->left =
                m_resizeCenterX -
                targetWindowWidth / 2;

            rect->right =
                rect->left + targetWindowWidth;
            break;

        case WMSZ_BOTTOM:
            /*
             * Keep the top edge fixed.
             * Keep the original horizontal center fixed.
             */
            rect->bottom =
                rect->top + targetWindowHeight;

            rect->left =
                m_resizeCenterX -
                targetWindowWidth / 2;

            rect->right =
                rect->left + targetWindowWidth;
            break;

        case WMSZ_TOPLEFT:
            /*
             * Keep the bottom-right corner fixed.
             */
            rect->left =
                rect->right - targetWindowWidth;

            rect->top =
                rect->bottom - targetWindowHeight;
            break;

        case WMSZ_TOPRIGHT:
            /*
             * Keep the bottom-left corner fixed.
             */
            rect->right =
                rect->left + targetWindowWidth;

            rect->top =
                rect->bottom - targetWindowHeight;
            break;

        case WMSZ_BOTTOMLEFT:
            /*
             * Keep the top-right corner fixed.
             */
            rect->left =
                rect->right - targetWindowWidth;

            rect->bottom =
                rect->top + targetWindowHeight;
            break;

        case WMSZ_BOTTOMRIGHT:
            /*
             * Keep the top-left corner fixed.
             */
            rect->right =
                rect->left + targetWindowWidth;

            rect->bottom =
                rect->top + targetWindowHeight;
            break;
        }

        if (result)
            *result = TRUE;

        return true;
    }

#endif

    return QQuickWindow::nativeEvent(
        eventType,
        message,
        result);
}
