
#include <gloperate-qtquick/TextureRenderItem.h>

#include <glm/glm.hpp>

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
#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>
#include <gloperate/pipeline/PipelineContainer.h>


namespace
{


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


}


using namespace gloperate;


namespace gloperate_qtquick
{


void TextureRenderItem::render()
{
    // Get texture
    if (!m_texture)
    {
        Canvas * canvas = static_cast<Canvas*>(m_environment->canvases()[0]);
        AbstractSlot * slot = canvas->pipelineContainer()->getSlot(m_path.toStdString());

        if (slot && slot->type() == typeid(globjects::Texture *))
        {
            m_texture = static_cast< Slot<globjects::Texture *> * >(slot)->value();
        }
    }

    if (!m_texture)
    {
        return;
    }

    // Check if geometry needs to be built
    if (!m_vao.get())
    {
        buildGeometry();
    }

    // Check if program needs to be (re-)built
    if (!m_program.get())
    {
        buildProgram();
    }

    // Bind default FBO
    globjects::Framebuffer::defaultFBO()->bind(gl::GL_FRAMEBUFFER);

    // Set viewport
    float dy = getWindowHeight() - (m_y + m_height);
    float x      = m_x      * m_devicePixelRatio;
    float y      = dy       * m_devicePixelRatio;
    float width  = m_width  * m_devicePixelRatio;
    float height = m_height * m_devicePixelRatio;
    gl::glViewport(x, y, width, height);

    // Disable depth test for screen-aligned quad
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Enable blending
    gl::glEnable(gl::GL_BLEND);

    // Restore OpenGL states
    gl::glEnable(gl::GL_DEPTH_TEST);

    // Bind texture
    gl::glActiveTexture(gl::GL_TEXTURE0 + 0);
    m_texture->bind();

    // Draw screen-aligned quad
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_vao->unbind();
    m_program->release();

    // Unbind texture
    m_texture->unbind();
}

void TextureRenderItem::buildGeometry()
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
    m_vao = cppassist::make_unique<globjects::VertexArray>();

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(buffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void TextureRenderItem::buildProgram()
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Create program and load shaders
    m_program = cppassist::make_unique<globjects::Program>();
    loadShader(m_program.get(), gl::GL_VERTEX_SHADER,   dataPath + "/gloperate/shaders/Mixer/Mixer.vert");
    loadShader(m_program.get(), gl::GL_FRAGMENT_SHADER, dataPath + "/gloperate/shaders/Mixer/Mixer.frag");

    // Set uniforms
    m_program->setUniform("texColor", 0);
}


} // namespace gloperate_qtquick
