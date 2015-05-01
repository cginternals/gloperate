#include "RotatingQuad.h"

#include <random>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/logging.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>


using namespace globjects;
using namespace gloperate;
using namespace gl;

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


RotatingQuad::RotatingQuad(ResourceManager & resourceManager)
: Painter(resourceManager)
, m_angle(0.0f)
{
    m_viewportCapability = addCapability(new gloperate::ViewportCapability());
    m_timeCapability = addCapability(new gloperate::VirtualTimeCapability());

    m_timeCapability->setLoopDuration(2.0f * glm::pi<float>());
}

RotatingQuad::~RotatingQuad()
{
}

void RotatingQuad::onInitialize()
{
#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

    gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    createAndSetupCamera();
    createAndSetupTexture();
    createAndSetupGeometry();
}

void RotatingQuad::onPaint()
{
    if (m_viewportCapability->hasChanged())
    {
        glViewport(m_viewportCapability->x(), m_viewportCapability->y(), m_viewportCapability->width(), m_viewportCapability->height());

        m_viewportCapability->setChanged(false);
    }

    // [TODO] Add onIdle()/onUpdate() callback and implement framerate independent animation
    m_angle = m_timeCapability->time();

    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0);
    model = glm::rotate(model, m_angle, glm::vec3(0.0, 1.0, 0.0));

    m_program->setUniform("viewProjectionMatrix",      m_camera->viewProjection());
    m_program->setUniform("modelViewProjectionMatrix", m_camera->viewProjection() * model);

    if (m_texture) {
        gl::glActiveTexture(gl::GL_TEXTURE0 + 0);
        m_texture->bind();
    }

    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    if (m_texture) {
        m_texture->unbind();
    }
}

void RotatingQuad::createAndSetupCamera()
{
    m_camera = new Camera();
    m_camera->setEye(glm::vec3(0.0, 0.0, 12.0));
}

void RotatingQuad::createAndSetupTexture()
{
    // Try to load texture
    m_texture = m_resourceManager.load<globjects::Texture>("data/emblem-important.png");

    // Check if texture is valid
    if (!m_texture) {
        // Create procedural texture
        static const int w(256);
        static const int h(256);
        unsigned char data[w * h * 4];

        std::random_device rd;
        std::mt19937 generator(rd());
        std::poisson_distribution<> r(0.2);

        for (int i = 0; i < w * h * 4; ++i) {
            data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));
        }

        m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
        m_texture->image2D(0, gl::GL_RGBA8, w, h, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data);
    }
}

void RotatingQuad::createAndSetupGeometry()
{
    static const std::array<glm::vec2, 4> raw {
        glm::vec2( +1.f, -1.f ),
        glm::vec2( +1.f, +1.f ),
        glm::vec2( -1.f, -1.f ),
        glm::vec2( -1.f, +1.f ) };

    m_vao = new VertexArray;
    m_buffer = new Buffer();
    m_buffer->setData(raw, gl::GL_STATIC_DRAW);

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_buffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);

    StringTemplate * vertexShaderSource   = new StringTemplate(new StaticStringSource(s_vertexShader  ));
    StringTemplate * fragmentShaderSource = new StringTemplate(new StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = new Shader(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    m_fragmentShader = new Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_program = new Program();
    m_program->attach(m_vertexShader, m_fragmentShader);

    m_program->setUniform("source", 0);
}
