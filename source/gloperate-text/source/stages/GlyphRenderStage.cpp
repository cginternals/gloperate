
#include <gloperate-text/stages/GlyphRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>
#include <globjects/base/AbstractStringSource.h>
#include <globjects/Shader.h>
#include <globjects/Program.h>

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
    m_vSource = GlyphRenderer::vertexShaderSource();
    m_gSource = GlyphRenderer::geometryShaderSource();
    m_fSource = GlyphRenderer::fragmentShaderSource();

    m_vertexShader = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER, m_vSource.get());
    m_geometryShader = cppassist::make_unique<globjects::Shader>(gl::GL_GEOMETRY_SHADER, m_gSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fSource.get());

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get());
    m_program->attach(m_geometryShader.get());
    m_program->attach(m_fragmentShader.get());

    m_renderer = cppassist::make_unique<GlyphRenderer>(m_program.get());
}

void GlyphRenderStage::onContextDeinit(gloperate::AbstractGLContext *)
{
}

void GlyphRenderStage::onProcess()
{
    gl::glViewport(viewport->x, viewport->y, viewport->z, viewport->w);

    auto fbo = targetFramebuffer.value();
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
