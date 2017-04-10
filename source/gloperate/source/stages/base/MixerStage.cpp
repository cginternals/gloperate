
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

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(MixerStage, gloperate::Stage)


MixerStage::MixerStage(Environment * environment, const std::string & name)
: Stage(environment, "MixerStage", name)
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
    m_vao = nullptr;
    m_buffer = nullptr;
    m_vertexShader = nullptr;
    m_geometryShader = nullptr;
    m_fragmentShader = nullptr;
    m_program = nullptr;
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
    assert(fbo);

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
        texture->bind();
    }

    // Draw screen-aligned quad
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_vao->unbind();
    m_program->release();

    // Unbind texture
    if (*texture) {
        texture->unbind();
    }

    // Unbind FBO, bind default FBO
    if (*targetFBO) {
        targetFBO->unbind(gl::GL_FRAMEBUFFER);
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
    m_buffer = cppassist::make_unique<globjects::Buffer>();
    m_buffer->setData(vertices, gl::GL_STATIC_DRAW); // needed for some drivers

    // Create VAO
    m_vao = cppassist::make_unique<globjects::VertexArray>();

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_buffer.get(), 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void MixerStage::buildProgram()
{
    // Create program and load shaders
    m_program = cppassist::make_unique<globjects::Program>();
    if (auto shader = environment()->resourceManager()->load<globjects::Shader>(vertexShader.value()))
    {
        m_vertexShader = std::unique_ptr<globjects::Shader>(shader);
        m_program->attach(m_vertexShader.get());
    }
    if (auto shader = environment()->resourceManager()->load<globjects::Shader>(geometryShader.value()))
    {
        m_geometryShader = std::unique_ptr<globjects::Shader>(shader);
        m_program->attach(m_geometryShader.get());
    }
    if (auto shader = environment()->resourceManager()->load<globjects::Shader>(fragmentShader.value()))
    {
        m_fragmentShader = std::unique_ptr<globjects::Shader>(shader);
        m_program->attach(m_fragmentShader.get());
    }

    // Set uniforms
    m_program->setUniform("texColor", 0);

    // Program has been built
    m_rebuildProgram = false;
}


} // namespace gloperate
