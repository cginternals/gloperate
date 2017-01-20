
#include <gloperate/stages/demos/DemoAntialiasableTriangleStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


// Geometry describing the triangle
static const std::array<glm::vec2, 3> s_vertices { {
    glm::vec2( +.2f, +.5f ),
    glm::vec2( +.5f, -.4f ),
    glm::vec2( -.5f, -.5f ) } };

// Vertex shader displaying the triangle
static const char * s_vertexShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    uniform vec2 offset;

    layout (location = 0) in vec2 a_vertex;

    out vec2 v_uv;

    void main()
    {
        gl_Position = vec4(a_vertex + offset, 0.0, 1.0);
    }
)";

// Fragment shader displaying the triangle
static const char * s_fragmentShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    layout (location = 0) out vec4 fragColor;

    void main()
    {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
)";


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoAntialiasableTriangleStage, gloperate::Stage)


DemoAntialiasableTriangleStage::DemoAntialiasableTriangleStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, subpixelOffset("subpixelOffset", this, glm::vec2(0.0f))
{
}

DemoAntialiasableTriangleStage::~DemoAntialiasableTriangleStage()
{
}

void DemoAntialiasableTriangleStage::onContextInit(AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void DemoAntialiasableTriangleStage::onContextDeinit(AbstractGLContext *)
{
}

void DemoAntialiasableTriangleStage::onProcess(AbstractGLContext *)
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
    m_program->setUniform("offset", *(subpixelOffset));

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 3);
    m_program->release();

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void DemoAntialiasableTriangleStage::onInputValueChanged(AbstractSlot * /*slot*/)
{
    renderInterface.rendered.setValid(false);
}

void DemoAntialiasableTriangleStage::setupGeometry()
{
    m_vao = new globjects::VertexArray;
    m_vertexBuffer = new globjects::Buffer();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void DemoAntialiasableTriangleStage::setupProgram()
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
