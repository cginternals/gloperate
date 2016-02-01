
#include <gloperate-text/stages/GlyphRenderStage.h>

#include <glbinding/gl/gl.h>

#include <glm/vec4.hpp>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>

#include <gloperate-text/GlyphRenderer.h>
#include <gloperate-text/geometry/GlyphVertexCloud.h>


namespace gloperate_text
{


GlyphRenderStage::GlyphRenderStage()
{
    addInput("vertices", vertices);
    addInput("fontColor", fontColor);
    addInput("distanceThreshold", distanceThreshold);

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
    if (fontColor.hasChanged())
    {
        glm::vec4 color = glm::vec4(fontColor.data().red(), fontColor.data().green(), fontColor.data().blue(), fontColor.data().alpha()) / 255.0f;
        m_renderer->program()->setUniform<glm::vec4>("fontColor", color);
    }

    if (distanceThreshold.hasChanged())
    {
        m_renderer->program()->setUniform<float>("distanceThreshold", distanceThreshold.data());
    }

    gl::glViewport(viewport.data()->x(), viewport.data()->y(), viewport.data()->width(), viewport.data()->height());

    globjects::Framebuffer * fbo = targetFramebuffer.data()->framebuffer();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    fbo->bind(gl::GL_FRAMEBUFFER);

    glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    m_renderer->render(vertices.data());

    fbo->unbind(gl::GL_FRAMEBUFFER);
}


} // namespace gloperate_text
