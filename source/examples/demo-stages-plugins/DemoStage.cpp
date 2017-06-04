
#include "DemoStage.h"

#include <random>

#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>


static const char * s_vertexShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    uniform mat4 modelViewProjectionMatrix;

    layout (location = 0) in vec3 a_vertex;
    layout (location = 1) in vec2 a_uv;

    out vec2 v_uv;

    void main()
    {
        v_uv = a_uv;
        gl_Position = modelViewProjectionMatrix * vec4(a_vertex, 1.0);
    }
)";

static const char * s_fragmentShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    uniform sampler2D source;

    layout (location = 0) out vec4 fragColor;

    in vec2 v_uv;

    void main()
    {
        fragColor = texture(source, v_uv);
    }
)";


CPPEXPOSE_COMPONENT(DemoStage, gloperate::Stage)


DemoStage::DemoStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "DemoStage", name)
, renderInterface(this)
, fboOut("fboOut", this, nullptr)
, m_angle(0.0f)
{
}

DemoStage::~DemoStage()
{
}

void DemoStage::onContextInit(gloperate::AbstractGLContext *)
{
    createAndSetupCamera();
    createAndSetupTexture();
    createAndSetupGeometry();
}

void DemoStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    m_quad           = nullptr;
    m_texture        = nullptr;
    m_program        = nullptr;
    m_vertexShader   = nullptr;
    m_fragmentShader = nullptr;
}

void DemoStage::onProcess()
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

    // Update animation
    m_angle = *renderInterface.virtualTime;

    // Clear background
    glm::vec3 color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Get model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::rotate(modelMatrix, m_angle, glm::vec3(0.0, 1.0, 0.0));

    // Update model-view-projection matrix
    m_program->setUniform("viewProjectionMatrix",      m_camera.viewProjectionMatrix());
    m_program->setUniform("modelViewProjectionMatrix", m_camera.viewProjectionMatrix() * modelMatrix);

    // Lazy creation of texture
    if (!m_texture) {
        createAndSetupTexture();
    }

    // Bind texture
    if (m_texture) {
        m_texture->bindActive(0);
    }

    // Draw geometry
    m_program->use();
    m_quad->draw();
    m_program->release();

    // Unbind texture
    if (m_texture) {
        m_texture->unbindActive(0);
    }

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
    fboOut.setValue(*renderInterface.targetFBO);
}

void DemoStage::createAndSetupCamera()
{
    m_camera.setEye(glm::vec3(0.0, 0.0, 12.0));
}

void DemoStage::createAndSetupTexture()
{
    // Load texture from file
    m_texture = std::unique_ptr<globjects::Texture>(m_environment->resourceManager()->load<globjects::Texture>(
        gloperate::dataPath() + "/gloperate/textures/gloperate-logo.glraw"
    ));
}

void DemoStage::createAndSetupGeometry()
{
    m_quad = cppassist::make_unique<gloperate::Sphere>(2.0f, gloperate::ShapeOption::IncludeTexCoords);

    // [TODO] This is a memory leak! Use resource loader?
    globjects::StringTemplate * vertexShaderSource   = new globjects::StringTemplate(new globjects::StaticStringSource(s_vertexShader  ));
    globjects::StringTemplate * fragmentShaderSource = new globjects::StringTemplate(new globjects::StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

    m_program->setUniform("source", 0);
}
