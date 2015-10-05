
#include "Logo.h"

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

#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>


using namespace reflectionzeug;
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


Logo::Logo(ResourceManager & resourceManager, const std::string & relDataPath)
: Painter("Logo", resourceManager, relDataPath)
, m_animation(true)
, m_background(0, 0, 0, 255)
, m_textureFilename(m_relDataPath + "data/logo/gloperate-logo.png")
, m_angle(0.0f)
{
    // Setup painter
    m_targetFramebufferCapability = addCapability(new gloperate::TargetFramebufferCapability());
    m_viewportCapability = addCapability(new gloperate::ViewportCapability());
    m_timeCapability = addCapability(new gloperate::VirtualTimeCapability());

    m_timeCapability->setLoopDuration(2.0f * glm::pi<float>());

    // Register properties
    addProperty<bool>("Animation", this, &Logo::animation, &Logo::setAnimation);
    addProperty<Color>("BackgroundColor", this, &Logo::background, &Logo::setBackground);
    addProperty<FilePath>("Texture", this, &Logo::texture, &Logo::setTexture);
}

Logo::~Logo()
{
}

bool Logo::animation() const
{
    return m_animation;
}

void Logo::setAnimation(const bool & enabled)
{
    m_animation = enabled;

    m_timeCapability->setEnabled(m_animation);
}

Color Logo::background() const
{
    return m_background;
}

void Logo::setBackground(const Color & color)
{
    m_background = color;
}

FilePath Logo::texture() const
{
    return m_textureFilename;
}

void Logo::setTexture(const FilePath & filename)
{
    m_textureFilename = filename;

    m_texture = nullptr;
}

void Logo::onInitialize()
{
#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

    createAndSetupCamera();
    createAndSetupTexture();
    createAndSetupGeometry();
}

void Logo::onPaint()
{
    if (m_viewportCapability->hasChanged())
    {
        glViewport(m_viewportCapability->x(), m_viewportCapability->y(), m_viewportCapability->width(), m_viewportCapability->height());

        m_viewportCapability->setChanged(false);
    }

    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    fbo->bind(GL_FRAMEBUFFER);

    // [TODO] Add onIdle()/onUpdate() callback and implement framerate independent animation
    m_angle = m_timeCapability->time();

    gl::glClearColor(
        (float)m_background.red()   / 255.0f,
        (float)m_background.green() / 255.0f,
        (float)m_background.blue()  / 255.0f,
        1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0);
    model = glm::rotate(model, m_angle, glm::vec3(0.0, 1.0, 0.0));

    m_program->setUniform("viewProjectionMatrix",      m_camera->viewProjection());
    m_program->setUniform("modelViewProjectionMatrix", m_camera->viewProjection() * model);

    if (!m_texture) {
        createAndSetupTexture();
    }

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

    globjects::Framebuffer::unbind(GL_FRAMEBUFFER);
}

void Logo::createAndSetupCamera()
{
    m_camera = new Camera();
    m_camera->setEye(glm::vec3(0.0, 0.0, 12.0));
}

void Logo::createAndSetupTexture()
{
    // Try to load texture
    m_texture = m_resourceManager.load<globjects::Texture>(m_textureFilename.toString());

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

void Logo::createAndSetupGeometry()
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
