
#include "AntialiasableTriangleStage.h"

#include <glbinding/gl/gl.h>

#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>


namespace
{

// Geometry describing the triangle
static const std::array<glm::vec2, 3> s_vertices { {
    glm::vec2( +.2f, +.5f ),
    glm::vec2( +.5f, -.4f ),
    glm::vec2( -.5f, -.5f ) } };

}


CPPEXPOSE_COMPONENT(AntialiasableTriangleStage, gloperate::Stage)


AntialiasableTriangleStage::AntialiasableTriangleStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, subpixelOffsets("subpixelOffset", this, nullptr)
{
}

AntialiasableTriangleStage::~AntialiasableTriangleStage()
{
}

void AntialiasableTriangleStage::onContextInit(gloperate::AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void AntialiasableTriangleStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();

    // deinitialize geometry
    m_vertexBuffer.reset();
    m_vao.reset();
}

void AntialiasableTriangleStage::onProcess()
{
    // Get viewport
    glm::vec4 viewport = *renderInterface.deviceViewport;

    // Update viewport
    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    // Bind FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    auto & color = *renderInterface.backgroundColor;
    gl::glClearColor(color.redf(), color.greenf(), color.bluef(), 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Set uniforms
    m_program->setUniform("offset", *subpixelOffsets
                                    ? (*subpixelOffsets)->at((*renderInterface.frameCounter) % (*subpixelOffsets)->size())
                                    : glm::vec2(0.0f));

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 3);
    m_program->release();

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void AntialiasableTriangleStage::onInputValueChanged(gloperate::AbstractSlot * /*slot*/)
{
    renderInterface.rendered.invalidate();
}

void AntialiasableTriangleStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void AntialiasableTriangleStage::setupProgram()
{
    m_vertexShader   = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/antialiasing.vert"));
    m_fragmentShader = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/antialiasing.frag"));

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
}
