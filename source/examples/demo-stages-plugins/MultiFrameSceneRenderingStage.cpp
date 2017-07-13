
#include "MultiFrameSceneRenderingStage.h"

#include <tuple>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/random.hpp>

#include <cppassist/logging/logging.h>

#include <glbinding/gl/gl.h>

#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>


CPPEXPOSE_COMPONENT(MultiFrameSceneRenderingStage, gloperate::Stage)


MultiFrameSceneRenderingStage::MultiFrameSceneRenderingStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, canvasInterface(this)
, subpixelShiftKernel("subpixelShiftKernel", this)
, dofShiftKernel("dofShiftKernel", this)
, transparencyKernelTexture("transparencyKernelTexture", this)
, projectionMatrix("projectionMatrix", this)
, normalMatrix("normalMatrix", this)
{
}

MultiFrameSceneRenderingStage::~MultiFrameSceneRenderingStage()
{
}

void MultiFrameSceneRenderingStage::onContextInit(gloperate::AbstractGLContext *)
{
    canvasInterface.onContextInit();
    setupGeometry();
    setupProgram();
}

void MultiFrameSceneRenderingStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();

    // deinitialize geometry
    if (m_drawable)
    {
        for (auto i : {0,1,2}) {
            delete m_drawable->buffer(i);
        }
        m_drawable.reset();
    }

    canvasInterface.onContextDeinit();
}

void MultiFrameSceneRenderingStage::onProcess()
{
    if (!m_drawable)
        return;

    // Get viewport
    const glm::vec4 & viewport = *canvasInterface.viewport;

    // Update viewport
    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    // Set uniforms
    auto viewMatrix = glm::lookAt(glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto projectionMatrix = glm::perspective(20.0f, viewport.z / viewport.w, 1.0f, 10.0f);
    auto viewProjectionMatrix = projectionMatrix * viewMatrix;

    auto subpixelShift = (*subpixelShiftKernel)->at((*canvasInterface.frameCounter) % (*subpixelShiftKernel)->size());
    subpixelShift *= glm::vec2(1.0f, 1.0f) / glm::vec2(viewport.z, viewport.w);
    auto dofShift = (*dofShiftKernel)->at((*canvasInterface.frameCounter) % (*dofShiftKernel)->size());

    this->projectionMatrix.setValue(projectionMatrix);
    this->normalMatrix.setValue(glm::inverseTranspose(glm::mat3(viewMatrix)));

    m_program->setUniform("viewMatrix", viewMatrix);
    m_program->setUniform("projectionMatrix", projectionMatrix);
    m_program->setUniform("subpixelShift", subpixelShift);
    m_program->setUniform("dofShift", dofShift);
    m_program->setUniform("currentFrame", *canvasInterface.frameCounter);

    // Bind color FBO
    globjects::Framebuffer * fbo = canvasInterface.obtainFBO();
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    auto & color = *canvasInterface.backgroundColor;
    gl::glClearColor(color.redf(), color.greenf(), color.bluef(), 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    // Set GL states
    gl::glDisable(gl::GL_SCISSOR_TEST);
    gl::glDisable(gl::GL_BLEND); // ..., as it is still enabled from previous frame

    // Bind textures
    if (*transparencyKernelTexture)
        (*transparencyKernelTexture)->bindActive(0);

    // Draw to color
    m_program->use();
    m_drawable->drawElements();
    m_program->release();

    // Unbind textures
    if (*transparencyKernelTexture)
        (*transparencyKernelTexture)->unbindActive(0);

    // Unbind color FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    canvasInterface.updateRenderTargetOutputs();
}

void MultiFrameSceneRenderingStage::setupGeometry()
{
    m_drawable = std::unique_ptr<gloperate::Drawable>{m_environment->resourceManager()->load<gloperate::Drawable>(gloperate::dataPath() + "/gloperate/meshes/Demo/MultiFrameDemo.obj")};

    if (!m_drawable)
    {
        cppassist::warning("demo-stages") << "Could not load mesh, stage will not render anything.";
    }
}

void MultiFrameSceneRenderingStage::setupProgram()
{
    m_vertexShader   = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/multi_frame_rendering.vert"));
    m_fragmentShader = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/multi_frame_rendering.frag"));

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

    m_program->setUniform("transparencyKernel", 0);
}
