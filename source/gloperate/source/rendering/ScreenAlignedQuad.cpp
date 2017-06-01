
#include <gloperate/rendering/ScreenAlignedQuad.h>

#include <glm/vec2.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Shader.h>
#include <globjects/base/File.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


ScreenAlignedQuad::ScreenAlignedQuad()
: m_drawable(cppassist::make_unique<Drawable>())
, m_buffer(cppassist::make_unique<globjects::Buffer>())
{
    static const std::array<glm::vec2, 4> raw{{
        glm::vec2( +1.f, -1.f )
    ,   glm::vec2( +1.f, +1.f )
    ,   glm::vec2( -1.f, -1.f )
    ,   glm::vec2( -1.f, +1.f )
    }};

    m_buffer->setData(raw, gl::GL_STATIC_DRAW);

    m_drawable->setPrimitiveMode(gl::GL_TRIANGLE_STRIP);
    m_drawable->setDrawMode(DrawMode::Arrays);
    m_drawable->bindAttribute(0, 0);

    m_drawable->setBuffer(0, m_buffer.get());

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
    return m_drawable.get();
}

void ScreenAlignedQuad::draw() const
{
    m_drawable->draw();
}

std::unique_ptr<globjects::AbstractStringSource> ScreenAlignedQuad::vertexShaderSource()
{
    return globjects::Shader::sourceFromFile(gloperate::dataPath()+"/gloperate/shaders/screenaligned/default.vert");
}

std::unique_ptr<globjects::AbstractStringSource> ScreenAlignedQuad::fragmentShaderSource()
{
    return globjects::Shader::sourceFromFile(gloperate::dataPath()+"/gloperate/shaders/screenaligned/default.frag");
}

std::unique_ptr<globjects::AbstractStringSource> ScreenAlignedQuad::fragmentShaderSourceInverted()
{
    return globjects::Shader::sourceFromFile(gloperate::dataPath()+"/gloperate/shaders/screenaligned/defaultInverted.frag");
}


} // namespace gloperate
