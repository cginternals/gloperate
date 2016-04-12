
#pragma once


#include <QQuickItem>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


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
class GLOPERATE_QTQUICK_API RenderItem : public QQuickItem
{
Q_OBJECT

public slots:
    void onCreateVideo(QString filename, int fps, int seconds, int width, int height);

public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent item (can be null)
    */
    RenderItem(QQuickItem * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderItem();

    /**
    *  @brief
    *    Get surface
    *
    *  @return
    *    Render surface (can be null)
    */
    gloperate::Surface * surface() const;


protected:
    void onWindowChanged(QQuickWindow * window);
    void onBeforeRendering();


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
};


} // namespace gloperate_qtquick
