#ifndef FIXED_ASPECT_RATIO_WINDOW_H
#define FIXED_ASPECT_RATIO_WINDOW_H

#include <QQuickWindow>

class FixedAspectRatioWindow : public QQuickWindow
{
    Q_OBJECT

    Q_PROPERTY(qreal aspectRatio
                   READ aspectRatio
                       WRITE setAspectRatio
                           NOTIFY aspectRatioChanged)

public:
    explicit FixedAspectRatioWindow(QWindow *parent = nullptr);

    qreal aspectRatio() const;
    void setAspectRatio(qreal ratio);

signals:
    void aspectRatioChanged();

protected:
    bool nativeEvent(const QByteArray &eventType,
                     void *message,
                     long *result) override;

private:
    qreal m_aspectRatio = 16.0 / 9.0;

    /*
     * The window center is captured when interactive resizing begins.
     * It remains unchanged throughout the complete resize operation.
     */
    int m_resizeCenterX = 0;
    int m_resizeCenterY = 0;
    bool m_resizeCenterValid = false;
};

#endif