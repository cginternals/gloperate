
#pragma once


#include <QQuickItem>
#include <QTimer>


class QQuickWindow;

namespace gloperate {
    class Surface;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item for displaying a gloperate render stage
*/
class RenderItem : public QQuickItem
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    */
    RenderItem();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderItem();


protected:
    void onWindowChanged(QQuickWindow * window);
    void onBeforeRendering();


protected slots:
    void onUpdate();


protected:
    virtual void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry) override;
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void keyReleaseEvent(QKeyEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;


protected:
    gloperate::Surface * m_surface;          ///< Surface that renders into the item (must NOT be null)
    float                m_devicePixelRatio; ///< Number of device pixels per virtual pixel
    bool                 m_initialized;      ///< 'true' if the surface has been initialized, else 'false'
    QTimer               m_timer;            ///< Timer for continuous update
};


} // namespace gloperate_qtquick
