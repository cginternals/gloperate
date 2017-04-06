
#pragma once

#include <memory>

#include <QString>
#include <QQuickFramebufferObject>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QQuickWindow;

namespace globjects
{
    class Framebuffer;
}

namespace gloperate
{
    class AbstractCanvas;
}

namespace gloperate_qt
{
    class GLContext;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item for displaying a gloperate render stage
*/
class GLOPERATE_QTQUICK_API RenderItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString stage READ stage WRITE setStage)


friend class RenderItemRenderer;


signals:
    void canvasInitialized();
    void updated();


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
    *    Get canvas
    *
    *  @return
    *    Canvas that renders into the item (can be null)
    */
    const gloperate::AbstractCanvas * canvas() const;

    /**
    *  @brief
    *    Get canvas
    *
    *  @return
    *    Canvas that renders into the item (can be null)
    */
    gloperate::AbstractCanvas * canvas();

    // Virtual QQuickFramebufferObject interface
    QQuickFramebufferObject::Renderer * createRenderer() const Q_DECL_OVERRIDE;


protected:
    const QString & stage() const;
    void setStage(const QString & name);
    void onWindowChanged(QQuickWindow * window);
    void doRender(int fboId);

protected slots:
    void doUpdate();


protected:
    virtual void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry) override;
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void keyReleaseEvent(QKeyEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;


protected:
    std::unique_ptr<gloperate_qt::GLContext>   m_context;          ///< Context wrapper for gloperate (can be null)
    std::unique_ptr<gloperate::AbstractCanvas> m_canvas;           ///< Canvas that renders into the item (must NOT be null)
    float                                      m_devicePixelRatio; ///< Number of device pixels per virtual pixel
    bool                                       m_initialized;      ///< 'true' if the canvas has been initialized, else 'false'
    QString                                    m_stage;            ///< Name of the render stage to use
};


} // namespace gloperate_qtquick
