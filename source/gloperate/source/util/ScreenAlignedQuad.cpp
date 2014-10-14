#include <gloperate/util/ScreenAlignedQuad.h>
#include <cassert>
#include <array>
#include <glbinding/gl/enum.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/Program.h>
#include <globjects/VertexArray.h>
#include <globjects/Texture.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Buffer.h>
#include <globjects/Shader.h>
#include <globjects/base/StringTemplate.h>


using namespace globjects;
namespace gloperate
{


const char * ScreenAlignedQuad::s_defaultVertexShaderSource = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 a_vertex;
out vec2 v_uv;

void main()
{
    v_uv = a_vertex * 0.5 + 0.5;
    gl_Position = vec4(a_vertex, 0.0, 1.0);
}
)";

const char* ScreenAlignedQuad::s_defaultFagmentShaderSource = R"(
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


ScreenAlignedQuad::ScreenAlignedQuad(Shader *fragmentShader, Texture *texture)
: m_vertexShader(nullptr)
, m_fragmentShader(fragmentShader)
, m_program(new Program())
, m_texture(texture)
, m_samplerIndex(0)
{
    StringTemplate* vertexShaderSource = new StringTemplate(new StaticStringSource(s_defaultVertexShaderSource));
    StringTemplate* fragmentShaderSource = new StringTemplate(new StaticStringSource(s_defaultFagmentShaderSource));

#ifdef MAC_OS
    vertexShaderSource->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = new Shader(gl::GL_VERTEX_SHADER, vertexShaderSource);

    if (!m_fragmentShader) {
        m_fragmentShader = new Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    }

    m_program->attach(m_vertexShader, m_fragmentShader);

    initialize();
}

ScreenAlignedQuad::ScreenAlignedQuad(Shader * fragmentShader)
: ScreenAlignedQuad(fragmentShader, nullptr)
{
}

ScreenAlignedQuad::ScreenAlignedQuad(Texture * texture)
: ScreenAlignedQuad(nullptr, texture)
{
}

ScreenAlignedQuad::ScreenAlignedQuad(Program * program)
: m_vertexShader(nullptr)
, m_fragmentShader(nullptr)
, m_program(program)
, m_texture(nullptr)
, m_samplerIndex(0)
{
    assert(program != nullptr);

    initialize();
}

void ScreenAlignedQuad::initialize()
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    static const std::array<glm::vec2, 4> raw{{
        glm::vec2( +1.f, -1.f )
    ,   glm::vec2( +1.f, +1.f )
    ,   glm::vec2( -1.f, -1.f )
    ,   glm::vec2( -1.f, +1.f )
    }};

    m_vao = new VertexArray;
    m_buffer = new Buffer();
    m_buffer->setData(raw, gl::GL_STATIC_DRAW); //needed for some drivers

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_buffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);

    setSamplerUniform(0);
}

void ScreenAlignedQuad::draw()
{
    if (m_texture) {
        gl::glActiveTexture(gl::GL_TEXTURE0 + m_samplerIndex);
        m_texture->bind();
    }

    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    if (m_texture) {
        m_texture->unbind();
    }
}

void ScreenAlignedQuad::setTexture(Texture* texture)
{
    m_texture = texture;
}

void ScreenAlignedQuad::setSamplerUniform(int index)
{
    m_samplerIndex = index;
    m_program->setUniform("source", m_samplerIndex);
}

VertexArray * ScreenAlignedQuad::vao() const
{
    return m_vao;
}

Program * ScreenAlignedQuad::program()
{
    return m_program;
}

Shader * ScreenAlignedQuad::vertexShader()
{
    return m_vertexShader;
}

Shader * ScreenAlignedQuad::fragmentShader()
{
    return m_fragmentShader;
}


} // gloperate
