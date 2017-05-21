
#pragma once


#include <memory>

#include <QQuickFramebufferObject>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace globjects
{
    class Framebuffer;
    class Texture;
    class Program;
    class Shader;
    class AbstractStringSource;
}

namespace gloperate
{
    class Canvas2;
    class ScreenAlignedQuad;
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
    void configureFbo(int fboId, unsigned int width, unsigned int height);
    void renderTexture();
    void initializeFboAttachments();


protected:
    RenderItem                                       * m_renderItem;           ///< RenderItem into which is rendered
    bool                                               m_contextInitialized;   ///< 'true' if context has been initialized, else 'false'
    bool                                               m_canvasInitialized;    ///< 'true' if canvas has been initialized, else 'false'
    unsigned int                                       m_width;                ///< Current width
    unsigned int                                       m_height;               ///< Current height
    gloperate::Canvas2                               * m_canvas;               ///< Canvas that renders into the item (never null)
    std::unique_ptr<gloperate_qt::GLContext>           m_context;              ///< Context wrapper for gloperate (can be null)
    std::unique_ptr<globjects::Framebuffer>            m_fbo;                  ///< Framebuffer wrapper for outer FBO
    std::unique_ptr<globjects::Framebuffer>            m_innerFbo;             ///< Framebuffer into which gloperate renders
    std::unique_ptr<globjects::Texture>                m_texColor;             ///< Color texture
    std::unique_ptr<globjects::Texture>                m_texDepth;             ///< Depth texture
    std::unique_ptr<gloperate::ScreenAlignedQuad>      m_screenAlignedQuad;    ///< Screen aligned quad geometry
    std::unique_ptr<globjects::Program>                m_program;              ///< Shader program
    std::unique_ptr<globjects::AbstractStringSource>   m_vertexShaderSource;   ///< Vertex shader source
    std::unique_ptr<globjects::AbstractStringSource>   m_fragmentShaderSource; ///< Fragment shader source
    std::unique_ptr<globjects::Shader>                 m_vertexShader;         ///< Vertex shader
    std::unique_ptr<globjects::Shader>                 m_fragmentShader;       ///< Fragment shader
};


} // namespace gloperate_qtquick
