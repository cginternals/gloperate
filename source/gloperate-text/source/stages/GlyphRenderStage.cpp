
#include <gloperate-text/stages/GlyphRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>

#include <gloperate-text/GlyphRenderer.h>
#include <gloperate-text/GlyphVertexCloud.h>


namespace gloperate_text
{


GlyphRenderStage::GlyphRenderStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "GlyphRenderStage", name)
, vertexCloud("vertexCloud", this)
, viewport("viewport", this)
, targetFramebuffer("targetFramebuffer", this)
, rendered("rendered", this)
{
}


GlyphRenderStage::~GlyphRenderStage()
{
}


void GlyphRenderStage::onContextInit(gloperate::AbstractGLContext *)
{
    m_renderer = std::unique_ptr<GlyphRenderer>{ new GlyphRenderer{} };
}


void GlyphRenderStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    m_renderer = nullptr;
}


void GlyphRenderStage::onProcess(gloperate::AbstractGLContext *)
{
    gl::glViewport(viewport->x, viewport->y, viewport->z, viewport->w);

    auto fbo = targetFramebuffer.value();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }
    fbo->bind();

    gl::glDepthMask(gl::GL_FALSE);
    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    m_renderer->render(*vertexCloud.value());
    
    gl::glDepthMask(gl::GL_TRUE);
    gl::glDisable(gl::GL_CULL_FACE);
    gl::glBlendFunc(gl::GL_ONE, gl::GL_ZERO);
    gl::glDisable(gl::GL_BLEND);

    fbo->unbind();

    rendered.setValue(true);
}


} // namespace gloperate_text
