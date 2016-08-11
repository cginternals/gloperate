
#include <gloperate/rendering/ScreenAlignedTriangle.h>

#include <glm/vec2.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Shader.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


ScreenAlignedTriangle::ScreenAlignedTriangle()
: m_drawable(new Drawable)
{
    static const std::array<glm::vec2, 4> raw{{
        glm::vec2( +1.f, -1.f )
    ,   glm::vec2( +1.f, +3.f )
    ,   glm::vec2( -3.f, -1.f )
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

ScreenAlignedTriangle::~ScreenAlignedTriangle()
{
}

const Drawable * ScreenAlignedTriangle::drawable() const
{
    return m_drawable;
}

void ScreenAlignedTriangle::draw() const
{
    m_drawable->draw();
}

const std::string ScreenAlignedTriangle::s_defaultVertexShaderSource = gloperate::dataPath() + "/shaders/screenaligned/default.vert";

const std::string ScreenAlignedTriangle::s_defaultFragmentShaderSource = gloperate::dataPath() + "/shaders/screenaligned/default.frag";

const globjects::Shader* ScreenAlignedTriangle::createDefaultVertexShader()
{
    return globjects::Shader::fromFile(gl::GL_VERTEX_SHADER, s_defaultVertexShaderSource);
}

const globjects::Shader* ScreenAlignedTriangle::createDefaultFragmentShader()
{
    return globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, s_defaultFragmentShaderSource);
}


} // namespace gloperate
