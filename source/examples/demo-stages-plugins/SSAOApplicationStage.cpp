
#include "SSAOApplicationStage.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/File.h>
#include <globjects/Framebuffer.h>
#include <globjects/NamedString.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


CPPEXPOSE_COMPONENT(SSAOApplicationStage, gloperate::Stage)


SSAOApplicationStage::SSAOApplicationStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, colorTexture("colorTexture", this, nullptr)
, normalTexture("normalTexture", this, nullptr)
, depthTexture("depthTexture", this, nullptr)
, ssaoKernel("ssaoKernel", this, nullptr)
, ssaoNoise("ssaoNoise", this, nullptr)
, projectionMatrix("projectionMatrix", this)
, normalMatrix("normalMatrix", this)
, sceneRendered("sceneRendered", this, false)
{
}

SSAOApplicationStage::~SSAOApplicationStage()
{
}

void SSAOApplicationStage::onContextInit(gloperate::AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void SSAOApplicationStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();
    m_ssaoFileNamedString.reset();

    // deinitialize geometry
    m_screenAlignedQuad.reset();
}

void SSAOApplicationStage::onProcess()
{
    if (!(*colorTexture && *normalTexture && *depthTexture && *ssaoKernel && *ssaoNoise))
    {
        renderInterface.rendered.setValue(false);
        return;
    }

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
    m_program->setUniform("projectionMatrix", *projectionMatrix);
    m_program->setUniform("projectionInverseMatrix", glm::inverse(*projectionMatrix));
    m_program->setUniform("normalMatrix", *normalMatrix);

    // Bind textures
    (*colorTexture) ->bindActive(0);
    (*normalTexture)->bindActive(1);
    (*depthTexture) ->bindActive(2);
    (*ssaoKernel)   ->bindActive(3);
    (*ssaoNoise)    ->bindActive(4);

    // Draw geometry
    m_program->use();
    m_screenAlignedQuad->draw();
    m_program->release();

    // Unbind textures
    (*colorTexture) ->unbindActive(0);
    (*normalTexture)->unbindActive(1);
    (*depthTexture) ->unbindActive(2);
    (*ssaoKernel)   ->unbindActive(3);
    (*ssaoNoise)    ->unbindActive(4);

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void SSAOApplicationStage::setupGeometry()
{
    m_screenAlignedQuad = cppassist::make_unique<gloperate::ScreenAlignedQuad>();
}

void SSAOApplicationStage::setupProgram()
{
    m_vertexShaderSource   = gloperate::ScreenAlignedQuad::vertexShaderSource();
    m_fragmentShaderSource = globjects::Shader::sourceFromFile(gloperate::dataPath()+"/gloperate/shaders/Demo/DemoSSAOPostprocessing.frag");

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif
    m_ssaoFileNamedString = globjects::NamedString::create("/gloperate/shaders/lighting/ssao.glsl", new globjects::File(gloperate::dataPath() + "/gloperate/shaders/lighting/ssao.glsl"));

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   m_vertexShaderSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

    m_program->setUniform("colorTexture", 0);
    m_program->setUniform("normalTexture", 1);
    m_program->setUniform("depthTexture", 2);
    m_program->setUniform("ssaoKernelTexture", 3);
    m_program->setUniform("ssaoNoiseTexture", 4);

}
