
#include <gloperate-text/stages/GlyphRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>
#include <globjects/base/AbstractStringSource.h>

#include <openll/GlyphRenderer.h>
#include <openll/GlyphVertexCloud.h>


namespace gloperate_text
{


GlyphRenderStage::GlyphRenderStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "GlyphRenderStage", name)
, renderInterface(this)
, vertexCloud("vertexCloud", this)
{
}

GlyphRenderStage::~GlyphRenderStage()
{
}

void GlyphRenderStage::onContextInit(gloperate::AbstractGLContext *)
{
    renderInterface.onContextInit();

    m_renderer = cppassist::make_unique<openll::GlyphRenderer>();
}

void GlyphRenderStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    renderInterface.onContextDeinit();
}

void GlyphRenderStage::onProcess()
{
    gl::glViewport(renderInterface.viewport->x, renderInterface.viewport->y, renderInterface.viewport->z, renderInterface.viewport->w);

    auto fbo = renderInterface.obtainFBO();
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

    renderInterface.updateRenderTargetOutputs();
}


} // namespace gloperate_text
