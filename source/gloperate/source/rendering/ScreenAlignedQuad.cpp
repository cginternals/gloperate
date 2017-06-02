
#include <gloperate/rendering/ScreenAlignedQuad.h>

#include <glm/vec2.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/base/File.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


std::unique_ptr<globjects::AbstractStringSource> ScreenAlignedQuad::vertexShaderSource()
{
    return globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/screenaligned/default.vert");
}

std::unique_ptr<globjects::AbstractStringSource> ScreenAlignedQuad::fragmentShaderSource()
{
    return globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/screenaligned/default.frag");
}

std::unique_ptr<globjects::AbstractStringSource> ScreenAlignedQuad::fragmentShaderSourceInverted()
{
    return globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/screenaligned/defaultInverted.frag");
}

ScreenAlignedQuad::ScreenAlignedQuad()
: m_texture(nullptr)
, m_inverted(false)
, m_initialized(false)
{
}

ScreenAlignedQuad::~ScreenAlignedQuad()
{
}

globjects::Texture * ScreenAlignedQuad::texture() const
{
    return m_texture;
}

void ScreenAlignedQuad::setTexture(globjects::Texture * texture)
{
    m_texture = texture;
}

bool ScreenAlignedQuad::inverted() const
{
    return m_inverted;
}

void ScreenAlignedQuad::setInverted(bool inverted)
{
    // Reset needed?
    if (m_initialized && m_inverted != inverted)
    {
        m_initialized = false;
    }

    m_inverted = inverted;
}

void ScreenAlignedQuad::draw() const
{
    // Check if texture is valid
    if (!m_texture) return;

    // Initialize OpenGL objects
    if (!m_initialized) {
        const_cast<ScreenAlignedQuad *>(this)->initialize();
    }

    // Bind texture
    gl::glActiveTexture(gl::GL_TEXTURE0 + 0);
    m_texture->bind();

    // Disable depth test for screen-aligned quad
    gl::glDisable(gl::GL_DEPTH_TEST);

    // Draw geometry
    m_program->use();
    m_drawable->draw();
    m_program->release();

    // Unbind texture
    m_texture->unbind();
}

void ScreenAlignedQuad::initialize()
{
    // Quad geometry
    static const std::array<glm::vec2, 4> vertices { {
        glm::vec2( +1.f, -1.f )
    ,   glm::vec2( +1.f, +1.f )
    ,   glm::vec2( -1.f, -1.f )
    ,   glm::vec2( -1.f, +1.f )
    } };

    // Create drawable
    m_drawable = cppassist::make_unique<Drawable>();
    m_drawable->setPrimitiveMode(gl::GL_TRIANGLE_STRIP);
    m_drawable->setDrawMode(DrawMode::Arrays);
    m_drawable->setSize(4);

    // Create vertex buffer
    m_vertices = cppassist::make_unique<globjects::Buffer>();
    m_vertices->setData(vertices, gl::GL_STATIC_DRAW);

    m_drawable->bindAttribute(0, 0);
    m_drawable->setBuffer(0, m_vertices.get());
    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec2));
    m_drawable->setAttributeBindingFormat(0, 2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_drawable->enableAttributeBinding(0);

    // Create vertex shader
    m_vertexShaderSource = vertexShaderSource();
    m_vertexShader = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER, m_vertexShaderSource.get());

    // Create fragment shader
    m_fragmentShaderSource = m_inverted ? fragmentShaderSourceInverted() : fragmentShaderSource();
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());

    // Create program
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
    m_program->setUniform("source", 0);

    // Done
    m_initialized = true;
}


} // namespace gloperate
