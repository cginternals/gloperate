
#include "ColorizeStage.h"

#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/base/File.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>

#include <gloperate/gloperate.h>
#include <gloperate/rendering/ScreenAlignedQuad.h>


CPPEXPOSE_COMPONENT(ColorizeStage, gloperate::Stage)


using namespace gloperate;


ColorizeStage::ColorizeStage(Environment * environment, const std::string & name)
: Stage(environment, "ColorizeStage", name)
, renderInterface(this)
, texture        ("texture",         this, nullptr)
, colorTexture   ("colorTexture",    this, nullptr)
, color          ("color",           this, Color(255, 255, 255, 255))
, fboOut         ("fboOut",          this, nullptr)
, colorTextureOut("colorTextureOut", this, nullptr)
{
}

ColorizeStage::~ColorizeStage()
{
}

void ColorizeStage::onContextInit(AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void ColorizeStage::onContextDeinit(AbstractGLContext *)
{
    m_quad                 = nullptr;
    m_program              = nullptr;
    m_vertexShader         = nullptr;
    m_vertexShaderSource   = nullptr;
    m_fragmentShader       = nullptr;
    m_fragmentShaderSource = nullptr;
}

void ColorizeStage::onProcess()
{
    // Activate FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    glm::vec4 viewport = *renderInterface.deviceViewport;
    gl::glViewport(viewport.x, viewport.y, viewport.z, viewport.w);

    // Disable depth test for screen-aligned quad
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Enable blending
    gl::glEnable(gl::GL_BLEND);

    // Bind texture
    if (*texture) {
        gl::glActiveTexture(gl::GL_TEXTURE0 + 0);
        (*texture)->bind();
    }

    // Update color
    m_program->setUniform("color", color->toVec4());

    // Draw screen-aligned quad
    m_program->use();
    m_quad->draw();
    m_program->release();

    // Enable depth buffer
    gl::glEnable(gl::GL_DEPTH_TEST);

    // Unbind texture
    if (*texture) {
        (*texture)->unbind();
    }

    // Unbind FBO, bind default FBO
    if (*renderInterface.targetFBO) {
        (*renderInterface.targetFBO)->unbind(gl::GL_FRAMEBUFFER);
        globjects::Framebuffer::defaultFBO()->bind(gl::GL_FRAMEBUFFER);
    }

    // Indicate change to the output FBO and texture
    fboOut.setValue(fbo);
    colorTextureOut.setValue(*colorTexture);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void ColorizeStage::setupGeometry()
{
    m_quad = cppassist::make_unique<Quad>(2.0f, (unsigned int)ShapeOption::IncludeTexCoords);
}

void ColorizeStage::setupProgram()
{
    // Create vertex shader
    m_vertexShaderSource = ScreenAlignedQuad::vertexShaderSource();
    m_vertexShader = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER, m_vertexShaderSource.get());

    // Create fragment shader
    m_fragmentShaderSource = globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/demo/colorize.frag");
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());

    // Create program
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
    m_program->setUniform("source", 0);
}
