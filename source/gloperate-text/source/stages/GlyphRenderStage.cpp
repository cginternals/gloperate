
#include <gloperate-text/stages/GlyphRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>

#include <gloperate-text/GlyphRenderer.h>
#include <gloperate-text/GlyphVertexCloud.h>


namespace gloperate_text
{


GlyphRenderStage::GlyphRenderStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, vertexCloud("vertexCloud", this)
, viewport("viewport", this)
, targetFramebuffer("targetFramebuffer", this)
{
    setAlwaysProcessed(true);
}

GlyphRenderStage::~GlyphRenderStage()
{
}

void GlyphRenderStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_renderer.reset(new GlyphRenderer);
}

double avg = 0.0;
uint32_t n = 0;

void GlyphRenderStage::onProcess(gloperate::AbstractGLContext * context)
{
    gl::glViewport((*viewport)[0], (*viewport)[1], (*viewport)[2], (*viewport)[3]);

    globjects::Framebuffer * fbo = (*targetFramebuffer);

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }
    fbo->bind(gl::GL_FRAMEBUFFER);

    gl::glClearColor(1.f, 1.f, 1.f, 1.f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    gl::glDepthMask(gl::GL_FALSE);
    gl::glEnable(gl::GL_CULL_FACE);
    gl::glEnable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);

    m_renderer->render((*vertexCloud));
    
    gl::glDepthMask(gl::GL_TRUE);
    gl::glDisable(gl::GL_CULL_FACE);
    gl::glDisable(gl::GL_BLEND);

    fbo->unbind(gl::GL_FRAMEBUFFER);
}


} // namespace gloperate_text
