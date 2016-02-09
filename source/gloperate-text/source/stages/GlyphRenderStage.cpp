
#include <gloperate-text/stages/GlyphRenderStage.h>

#include <glbinding/gl/gl.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>

#include <gloperate-text/GlyphRenderer.h>
#include <gloperate-text/GlyphVertexCloud.h>


namespace gloperate_text
{


GlyphRenderStage::GlyphRenderStage()
{
    addInput("vertexCloud", vertexCloud);

    addInput("viewport", viewport);
    addInput("targetFramebuffer", targetFramebuffer);

    alwaysProcess(true);
}

GlyphRenderStage::~GlyphRenderStage()
{
}

void GlyphRenderStage::initialize()
{
    m_renderer.reset(new GlyphRenderer);
}

void GlyphRenderStage::process()
{
    gl::glViewport(viewport.data()->x(), viewport.data()->y(), viewport.data()->width(), viewport.data()->height());

    globjects::Framebuffer * fbo = targetFramebuffer.data()->framebuffer();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    fbo->bind(gl::GL_FRAMEBUFFER);

    glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    m_renderer->render(vertexCloud.data());

    fbo->unbind(gl::GL_FRAMEBUFFER);
}


} // namespace gloperate_text
