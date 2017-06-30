
#include "MultiFramePostprocessingStage.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/File.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/NamedString.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>


namespace
{


std::array<glm::vec2, 4> s_quad {{
    glm::vec2(-1.0f, -1.0f),
    glm::vec2( 1.0f, -1.0f),
    glm::vec2(-1.0f,  1.0f),
    glm::vec2( 1.0f,  1.0f)
}};


} // namespace


CPPEXPOSE_COMPONENT(MultiFramePostprocessingStage, gloperate::Stage)


MultiFramePostprocessingStage::MultiFramePostprocessingStage(gloperate::Environment * environment, const std::string & name)
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

MultiFramePostprocessingStage::~MultiFramePostprocessingStage()
{
}

void MultiFramePostprocessingStage::onContextInit(gloperate::AbstractGLContext *)
{
    if (m_vao) // protect against initializing twice
        return;

    setupGeometry();
    setupProgram();
}

void MultiFramePostprocessingStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();
    m_ssaoFileNamedString.reset();

    // deinitialize geometry
    m_vertexBuffer.reset();
    m_vao.reset();
}

void MultiFramePostprocessingStage::onProcess()
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
    auto & color = *renderInterface.backgroundColor;
    gl::glClearColor(color.redf(), color.greenf(), color.bluef(), 1.0f);
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
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
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

void MultiFramePostprocessingStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_quad, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec2));
    positionBinding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void MultiFramePostprocessingStage::setupProgram()
{
    m_ssaoFileNamedString = globjects::NamedString::create("/gloperate/shaders/lighting/ssao.glsl", new globjects::File(gloperate::dataPath() + "/gloperate/shaders/lighting/ssao.glsl"));

    m_vertexShader   = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath()+"/gloperate/shaders/geometry/screenaligned.vert"));
    m_fragmentShader = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath()+"/gloperate/shaders/demos/DemoSSAOPostprocessing.frag"));

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

    m_program->setUniform("colorTexture", 0);
    m_program->setUniform("normalTexture", 1);
    m_program->setUniform("depthTexture", 2);
    m_program->setUniform("ssaoKernelTexture", 3);
    m_program->setUniform("ssaoNoiseTexture", 4);
}
