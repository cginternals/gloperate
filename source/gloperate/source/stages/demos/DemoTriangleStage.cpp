
#include <gloperate/stages/demos/DemoTriangleStage.h>

#include <random>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/base/ResourceManager.h>


// Geometry describing the triangle
static const std::array<glm::vec2, 4> s_vertices { {
    glm::vec2( +1.f, -1.f ),
    glm::vec2( +1.f, +1.f ),
    glm::vec2( -1.f, -1.f ),
    glm::vec2( -1.f, +1.f ) } };

// Vertex shader displaying the triangle
static const char * s_vertexShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    uniform mat4 modelViewProjectionMatrix;

    layout (location = 0) in vec2 a_vertex;
    out vec2 v_uv;

    void main()
    {
        v_uv = a_vertex * 0.5 + 0.5;
        gl_Position = modelViewProjectionMatrix * vec4(a_vertex, 0.0, 1.0);
    }
)";

// Fragment shader displaying the triangle
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


namespace gloperate
{


DemoTriangleStage::DemoTriangleStage(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: RenderStage(viewerContext, name, parent)
, texture(this, "texture", nullptr)
, angle  (this, "angle",   0.0f)
{
}

DemoTriangleStage::~DemoTriangleStage()
{
}

void DemoTriangleStage::invalidateOutput()
{
    rendered.setValue(false);
}

void DemoTriangleStage::onContextInit(AbstractGLContext *)
{
    setupGeometry();
    setupCamera();
    setupProgram();
}

void DemoTriangleStage::onContextDeinit(AbstractGLContext *)
{
}

void DemoTriangleStage::onProcess(AbstractGLContext *)
{
    // Get viewport
    glm::vec4 viewport = *deviceViewport;

    // Update viewport
    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    // Bind FBO
    globjects::Framebuffer * fbo = *targetFBO;
    if (!fbo) fbo = globjects::Framebuffer::defaultFBO();
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    glm::vec3 color = *backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Get model matrix
    glm::mat4 model = glm::mat4(1.0);
    model = glm::rotate(model, *angle, glm::vec3(0.0, 1.0, 0.0));

    // Update model-view-projection matrix
    m_program->setUniform("viewProjectionMatrix",      m_camera->viewProjection());
    m_program->setUniform("modelViewProjectionMatrix", m_camera->viewProjection() * model);

    // Bind texture
    if (*texture) {
        gl::glActiveTexture(gl::GL_TEXTURE0 + 0);
        (*texture)->bind();
    }

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    // Unbind texture
    if (*texture) {
        (*texture)->unbind();
    }

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    //rendered.setValue(true);

    // Trigger immediate redraw
    invalidateOutput();
}

void DemoTriangleStage::setupGeometry()
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

void DemoTriangleStage::setupCamera()
{
    m_camera = new Camera();
    m_camera->setEye(glm::vec3(0.0, 0.0, 12.0));
}

void DemoTriangleStage::setupProgram()
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


CPPEXPOSE_COMPONENT(
    DemoTriangleStage, gloperate::Stage
  , "RenderStage"   // Tags
  , ""              // Icon
  , ""              // Annotations
  , "Demo stage that renders a rotating triangle onto the screen"
  , GLOPERATE_AUTHOR_ORGANIZATION
  , "v1.0.0"
)


} // namespace gloperate
