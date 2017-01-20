
#include <gloperate/stages/demos/DemoDOFCubeStage.h>

#include <array>

#include <glm/gtc/matrix_transform.hpp>

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


// Geometry describing the cube
// position, color
static const std::array<std::array<glm::vec3,2>, 14> s_vertices { {
    {{ glm::vec3(-1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(-1.f, -1.f, +1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(+1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(+1.f, -1.f, +1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(+1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.7f) }},
    {{ glm::vec3(-1.f, -1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(-1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(-1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.1f) }},
    {{ glm::vec3(-1.f, +1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.1f) }},
    {{ glm::vec3(+1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(+1.f, +1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(+1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.7f) }},
    {{ glm::vec3(-1.f, +1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.0f) }},
    {{ glm::vec3(-1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.0f) }}
} };

// Vertex shader displaying the cube
static const char * s_vertexShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 view;
uniform mat4 projection;
uniform vec2 dofShift;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_color;

flat out vec3 v_color;

void main()
{
    vec4 viewPos = view * vec4(a_vertex, 1.0);
    vec4 shiftedPos = vec4(viewPos.xy + dofShift * (viewPos.z + 0.1), viewPos.zw);
    //shiftedPos = viewPos;
    gl_Position = projection * shiftedPos;

    v_color = a_color;
}
)";

// Fragment shader displaying the cube
static const char * s_fragmentShader = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

flat in vec3 v_color;

layout (location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(v_color, 1.0);
}
)";


} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoDOFCubeStage, gloperate::Stage)


DemoDOFCubeStage::DemoDOFCubeStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, dofShift("dofShift", this, glm::vec2(0.0f))
{
}

DemoDOFCubeStage::~DemoDOFCubeStage()
{
}

void DemoDOFCubeStage::onContextInit(AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void DemoDOFCubeStage::onContextDeinit(AbstractGLContext *)
{
}

void DemoDOFCubeStage::onProcess(AbstractGLContext *)
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
    if (!fbo) fbo = globjects::Framebuffer::defaultFBO();
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    glm::vec3 color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Set uniforms
    m_program->setUniform("dofShift", *(dofShift));
    auto view = glm::lookAt(glm::vec3(1.02f, -1.02f, 1.1f), glm::vec3(0.5f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto projection = glm::perspective(30.0f, viewport.z / viewport.w, 0.1f, 10.0f);
    m_program->setUniform("view", view);
    m_program->setUniform("projection", projection);

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 14);
    m_program->release();

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void DemoDOFCubeStage::setupGeometry()
{
    m_vao = new globjects::VertexArray;
    m_vertexBuffer = new globjects::Buffer();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3) * 2);
    positionBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);

    auto colorBinding = m_vao->binding(1);
    colorBinding->setAttribute(1);
    colorBinding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3) * 2);
    colorBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec3));
    m_vao->enable(1);
}

void DemoDOFCubeStage::setupProgram()
{
    globjects::StringTemplate * vertexShaderSource   = new globjects::StringTemplate(new globjects::StaticStringSource(s_vertexShader  ));
    globjects::StringTemplate * fragmentShaderSource = new globjects::StringTemplate(new globjects::StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = new globjects::Shader(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    m_fragmentShader = new globjects::Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_program = new globjects::Program();
    m_program->attach(m_vertexShader, m_fragmentShader);
}


} // namespace gloperate
