
#include <gloperate/stages/base/MixerStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/Framebuffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate.h>


/**
*  @brief
*    Load shader
*
*  @param[in] program
*    Shader program
*  @param[in] type
*    Shader type (e.g., GL_VERTEX_SHADER)
*  @param[in] filename
*    Shader file
*/
static void loadShader(globjects::Program * program, const gl::GLenum type, const std::string & filename)
{
    // Load shader
    globjects::StringTemplate * source = new globjects::StringTemplate(new globjects::File(filename));
    globjects::Shader * shader = new globjects::Shader(type, source);
    program->attach(shader);
}


namespace gloperate
{


CPPEXPOSE_COMPONENT(MixerStage, gloperate::Stage)


MixerStage::MixerStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, viewport      ("viewport", this)
, targetFBO     ("targetFBO", this)
, texture       ("texture", this)
, vertexShader  ("vertexShader", this)
, geometryShader("geometryShader", this)
, fragmentShader("fragmentShader", this)
, rendered      ("rendered", this)
, fboOut        ("fboOut", this)
, m_rebuildProgram(false)
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Set default values
    vertexShader  .setValue(dataPath + "/gloperate/shaders/Mixer/Mixer.vert");
    fragmentShader.setValue(dataPath + "/gloperate/shaders/Mixer/Mixer.frag");
}

MixerStage::~MixerStage()
{
}

void MixerStage::onContextInit(AbstractGLContext *)
{
}

void MixerStage::onContextDeinit(AbstractGLContext *)
{
}

void MixerStage::onProcess(AbstractGLContext *)
{
    // Check if geometry needs to be built
    if (!m_vao.get())
    {
        buildGeometry();
    }

    // Check if program needs to be (re-)built
    if (!m_program.get() || m_rebuildProgram)
    {
        buildProgram();
    }

    // Activate FBO
    globjects::Framebuffer * fbo = *targetFBO;
    fbo = fbo ? fbo : globjects::Framebuffer::defaultFBO();
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    gl::glViewport(viewport->x, viewport->y, viewport->z, viewport->w);

    // Disable depth test for screen-aligned quad
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Enable blending
    gl::glEnable(gl::GL_BLEND);

    // Restore OpenGL states
    gl::glEnable(gl::GL_DEPTH_TEST);

    // Bind texture
    if (*texture) {
        gl::glActiveTexture(gl::GL_TEXTURE0 + 0);
        (*texture)->bind();
    }

    // Draw screen-aligned quad
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_vao->unbind();
    m_program->release();

    // Unbind texture
    if (*texture) {
        (*texture)->unbind();
    }

    // Unbind FBO, bind default FBO
    if (*targetFBO) {
        (*targetFBO)->unbind(gl::GL_FRAMEBUFFER);
        globjects::Framebuffer::defaultFBO()->bind(gl::GL_FRAMEBUFFER);
    }

    // Indicate change to the output FBO
    fboOut.setValue(fbo);

    // Signal that output is valid
    rendered.setValue(true);
}

void MixerStage::onInputValueChanged(AbstractSlot * slot)
{
    // Rebuild program when shader files have changed
    if (slot == &vertexShader || slot == &fragmentShader)
    {
        m_rebuildProgram = true;
    }

    // Invalidate all outputs
    invalidateOutputs();
}

void MixerStage::buildGeometry()
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

void MixerStage::buildProgram()
{
    // Create program and load shaders
    m_program = new globjects::Program;
    if (vertexShader.value() != "") {
        loadShader(m_program, gl::GL_VERTEX_SHADER, vertexShader.value());
    }
    if (geometryShader.value() != "") {
        loadShader(m_program, gl::GL_GEOMETRY_SHADER, geometryShader.value());
    }
    if (fragmentShader.value() != "") {
        loadShader(m_program, gl::GL_FRAGMENT_SHADER, fragmentShader.value());
    }

    // Set uniforms
    m_program->setUniform("texColor", 0);

    // Program has been built
    m_rebuildProgram = false;
}


} // namespace gloperate
