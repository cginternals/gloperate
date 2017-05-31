
#include "DemoSSAORenderingStage.h"

#include <tuple>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


namespace
{


// Geometry, a triangle fan spanning 3 sides of a cube
static const std::array<std::array<glm::vec3, 2>, 8> s_vertices { {
    {{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(1.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(0.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f) }},
    {{ glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 0.f, 0.f) }},
    {{ glm::vec3(1.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) }},
    {{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f) }}
} };

// Vertex shader displaying the cube
static const char * s_vertexShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 modelViewProjection;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;

flat out vec3 v_normal;

void main()
{
    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);
    v_normal = a_normal;
}
)";

// Fragment shader displaying the cube
static const char * s_fragmentShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

const vec3 lightIntensity = vec3(0.9);

flat in vec3 v_normal;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec3 normal;

void main()
{
    fragColor = vec4(vec3(dot(v_normal, lightIntensity)), 1.0);
    normal = v_normal;
}
)";


} // namespace


CPPEXPOSE_COMPONENT(DemoSSAORenderingStage, gloperate::Stage)


DemoSSAORenderingStage::DemoSSAORenderingStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, projectionMatrix("projectionMatrix", this)
, normalMatrix("normalMatrix", this)
{
}

DemoSSAORenderingStage::~DemoSSAORenderingStage()
{
}

void DemoSSAORenderingStage::onContextInit(gloperate::AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void DemoSSAORenderingStage::onContextDeinit(gloperate::AbstractGLContext *)
{
}

void DemoSSAORenderingStage::onProcess()
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

    // Set uniforms
    auto view = glm::lookAt(glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto projection = glm::perspective(20.0f, viewport.z / viewport.w, 1.0f, 10.0f);
    auto viewProjection = projection * view;

    projectionMatrix.setValue(projection);
    normalMatrix.setValue(glm::inverseTranspose(glm::mat3(view)));

    m_program->setUniform("modelViewProjection", viewProjection);

    // Bind color FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    glm::vec3 color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);
    gl::glDisable(gl::GL_BLEND); // ..., as it is still enabled from previous frame

    // Draw to color
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_FAN, 0, 8);
    m_program->release();

    // Unbind color FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void DemoSSAORenderingStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    positionBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);

    auto normalBinding = m_vao->binding(1);
    normalBinding->setAttribute(1);
    normalBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    normalBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec3));
    m_vao->enable(1);
}

void DemoSSAORenderingStage::setupProgram()
{
    //TODO this is a memory leak! Use resource loader?
    globjects::StringTemplate * vertexShaderSource   = new globjects::StringTemplate(new globjects::StaticStringSource(s_vertexShader  ));
    globjects::StringTemplate * colorFragmentShaderSource = new globjects::StringTemplate(new globjects::StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    colorFragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER, vertexShaderSource);
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, colorFragmentShaderSource);

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
}
