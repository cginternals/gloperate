
#include <gloperate/stages/demos/ColorizeStage.h>

#include <glm/glm.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>


// Vertex shader displaying the triangle
static const char * s_vertexShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    layout (location = 0) in vec2 vertex;

    out vec2 v_uv;

    void main()
    {
        v_uv = vertex * 0.5 + 0.5;
        gl_Position = vec4(vertex, 0.0, 1.0);
    }
)";

// Fragment shader displaying the triangle
static const char * s_fragmentShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    uniform sampler2D source;
    uniform vec4      color;

    layout (location = 0) out vec4 fragColor;

    in vec2 v_uv;

    void main()
    {
        fragColor = texture(source, v_uv) * color;
    }
)";


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorizeStage, gloperate::Stage)


ColorizeStage::ColorizeStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, texture        ("texture",         this, nullptr)
, colorTexture   ("colorTexture",    this, nullptr)
, color          ("color",           this, glm::vec4(1.0))
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
}

void ColorizeStage::onProcess(AbstractGLContext *)
{
    // Activate FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    if (!fbo) fbo = globjects::Framebuffer::defaultFBO();
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
    m_program->setUniform("color", *color);

    // Draw screen-aligned quad
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_vao->unbind();
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
    // Static vertices
    static const std::array<glm::vec2, 4> vertices { {
          glm::vec2( +1.f, -1.f )
        , glm::vec2( +1.f, +1.f )
        , glm::vec2( -1.f, -1.f )
        , glm::vec2( -1.f, +1.f ) } };

    // Create vertex buffer
    globjects::Buffer * buffer = new globjects::Buffer();
    buffer->setData(vertices, gl::GL_STATIC_DRAW); // needed for some drivers

    // Create VAO
    m_vao = new globjects::VertexArray;

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(buffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void ColorizeStage::setupProgram()
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

    m_program->setUniform("source", 0);
}


} // namespace gloperate
