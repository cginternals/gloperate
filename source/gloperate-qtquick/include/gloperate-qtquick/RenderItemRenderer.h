
#pragma once


#include <memory>

#include <QQuickFramebufferObject>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


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


class RenderItem;


/**
*  @brief
*    Renderer that executes the rendering into the FBO
*/
class GLOPERATE_QTQUICK_API RenderItemRenderer : public QQuickFramebufferObject::Renderer
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] renderItem
    *    RenderItem into which is rendered (must NOT be null!)
    */
    RenderItemRenderer(RenderItem * renderItem);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderItemRenderer();

    // Virtual QQuickFramebufferObject::Renderer interface
    void synchronize(QQuickFramebufferObject * framebufferObject) Q_DECL_OVERRIDE;
    QOpenGLFramebufferObject * createFramebufferObject(const QSize & size) Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;


protected:
    void createFboWrapper(int fboId);
    void destroyFboWrapper();


protected:
    RenderItem                                 * m_renderItem;         ///< RenderItem into which is rendered (never null)
    globjects::Framebuffer                     * m_fbo;                ///< Framebuffer wrapper for gloperate (can be null)
    bool                                         m_contextInitialized; ///< 'true' if context has been initialized, else 'false'
    bool                                         m_canvasInitialized;  ///< 'true' if canvas has been initialized, else 'false'
    std::unique_ptr<gloperate_qt::GLContext>     m_context;            ///< Context wrapper for gloperate (can be null)
    std::shared_ptr<gloperate::AbstractCanvas>   m_canvas;             ///< Canvas that renders into the item (must NOT be null)
};


} // namespace gloperate_qtquick
