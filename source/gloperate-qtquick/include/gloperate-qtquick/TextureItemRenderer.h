
#pragma once


#include <memory>

#include <QQuickFramebufferObject>
#include <QString>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


namespace globjects
{
    class Framebuffer;
}

namespace gloperate
{
    class Environment;
    class ScreenAlignedQuad;
}


namespace gloperate_qtquick
{


class TextureItem;


/**
*  @brief
*    Renderer that executes the rendering into the FBO
*/
class GLOPERATE_QTQUICK_API TextureItemRenderer : public QQuickFramebufferObject::Renderer
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] textureItem
    *    TextureItem into which is rendered (must NOT be null!)
    *  @param[in] textureItem
    *    GlOperate environment (must NOT be null!)
    */
    TextureItemRenderer(TextureItem * textureItem, gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureItemRenderer();

    // Virtual QQuickFramebufferObject::Renderer interface
    void synchronize(QQuickFramebufferObject * framebufferObject) Q_DECL_OVERRIDE;
    QOpenGLFramebufferObject * createFramebufferObject(const QSize & size) Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;


protected:
    void configureFbo(int fboId, unsigned int width, unsigned int height);
    void renderTexture();
    void buildGeometry();
    void buildProgram();


protected:
    TextureItem                                 * m_textureItem;       ///< TextureItem into which is rendered
    gloperate::Environment                      * m_environment;       ///< GlOperate environment
    unsigned int                                  m_width;             ///< Current width
    unsigned int                                  m_height;            ///< Current height
    QString                                       m_path;              ///< Path to texture slot

    std::unique_ptr<globjects::Framebuffer>       m_fbo;               ///< Framebuffer wrapper for outer FBO
    std::unique_ptr<gloperate::ScreenAlignedQuad> m_screenAlignedQuad; ///< Screen aligned quad
};


} // namespace gloperate_qtquick
