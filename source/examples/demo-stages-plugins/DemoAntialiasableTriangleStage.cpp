
#include "DemoAntialiasableTriangleStage.h"

#include <glbinding/gl/gl.h>

#include <globjects/base/File.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


namespace
{

// Geometry describing the triangle
static const std::array<glm::vec2, 3> s_vertices { {
    glm::vec2( +.2f, +.5f ),
    glm::vec2( +.5f, -.4f ),
    glm::vec2( -.5f, -.5f ) } };

}


CPPEXPOSE_COMPONENT(DemoAntialiasableTriangleStage, gloperate::Stage)


DemoAntialiasableTriangleStage::DemoAntialiasableTriangleStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, subpixelOffsets("subpixelOffset", this, nullptr)
{
}

DemoAntialiasableTriangleStage::~DemoAntialiasableTriangleStage()
{
}

void DemoAntialiasableTriangleStage::onContextInit(gloperate::AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void DemoAntialiasableTriangleStage::onContextDeinit(gloperate::AbstractGLContext *)
{
}

void DemoAntialiasableTriangleStage::onProcess()
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
    glm::vec3 color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
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

void DemoAntialiasableTriangleStage::onInputValueChanged(gloperate::AbstractSlot * /*slot*/)
{
    renderInterface.rendered.invalidate();
}

void DemoAntialiasableTriangleStage::setupGeometry()
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

void DemoAntialiasableTriangleStage::setupProgram()
{
    m_vertexShaderSource   = globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/Demo/DemoAntialiasing.vert");
    m_fragmentShaderSource = globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/Demo/DemoAntialiasing.frag");

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   m_vertexShaderSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
}
