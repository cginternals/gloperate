
#include <gloperate/rendering/ScreenAlignedQuad.h>

#include <glm/vec2.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Shader.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


ScreenAlignedQuad::ScreenAlignedQuad()
: m_drawable(new Drawable)
{
    static const std::array<glm::vec2, 4> raw{{
        glm::vec2( +1.f, -1.f )
    ,   glm::vec2( +1.f, +1.f )
    ,   glm::vec2( -1.f, -1.f )
    ,   glm::vec2( -1.f, +1.f )
    }};

    globjects::Buffer * buffer = new globjects::Buffer;
    buffer->setData(raw, gl::GL_STATIC_DRAW);

    m_drawable->setPrimitiveMode(gl::GL_TRIANGLE_STRIP);
    m_drawable->setDrawMode(DrawMode::Arrays);
    m_drawable->bindAttribute(0, 0);

    m_drawable->setBuffer(0, buffer);

    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec2));
    m_drawable->setAttributeBindingFormat(0, 2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_drawable->enableAttributeBinding(0);

    m_drawable->setSize(4);
}

ScreenAlignedQuad::~ScreenAlignedQuad()
{
}

const Drawable * ScreenAlignedQuad::drawable() const
{
    return m_drawable;
}

void ScreenAlignedQuad::draw() const
{
    m_drawable->draw();
}

const std::string ScreenAlignedQuad::s_defaultVertexShaderSource = gloperate::dataPath() + "/gloperate/shaders/screenaligned/default.vert";

const std::string ScreenAlignedQuad::s_defaultFragmentShaderSource = gloperate::dataPath() + "/gloperate/shaders/screenaligned/default.frag";

globjects::Shader* ScreenAlignedQuad::createDefaultVertexShader()
{
    return globjects::Shader::fromFile(gl::GL_VERTEX_SHADER, s_defaultVertexShaderSource);
}

globjects::Shader* ScreenAlignedQuad::createDefaultFragmentShader()
{
    return globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, s_defaultFragmentShaderSource);
}


} // namespace gloperate
