
#include "FFMPEGVideoExporter.h"

#include <glm/vec2.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>
#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>
#include <gloperate/base/AbstractCanvas.h>
#include <gloperate/base/AbstractGLContext.h>


using namespace globjects;
using namespace gloperate;


static const char * s_vertexShader = R"(
    #version 140
    #extension GL_ARB_explicit_attrib_location : require

    layout (location = 0) in vec2 a_vertex;
    out vec2 v_uv;

    void main()
    {
        v_uv = a_vertex * 0.5 + 0.5;
        v_uv.y = 1.0 - v_uv.y;
        gl_Position = vec4(a_vertex, 0.0, 1.0);
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


CPPEXPOSE_COMPONENT(FFMPEGVideoExporter, gloperate::AbstractVideoExporter)


FFMPEGVideoExporter::FFMPEGVideoExporter()
: m_videoEncoder(new FFMPEGVideoEncoder)
, m_canvas(nullptr)
, m_image(nullptr)
, m_progress(0)
, m_initialized(false)
, m_contextHandling(AbstractVideoExporter::IgnoreContext)
{
}

FFMPEGVideoExporter::~FFMPEGVideoExporter()
{
    delete m_videoEncoder;
    if (m_image) delete m_image;
}

void FFMPEGVideoExporter::setTarget(gloperate::AbstractCanvas * canvas, const cppexpose::VariantMap & parameters)
{
    // Save configuration
    m_canvas     = canvas;
    m_parameters = parameters;
    m_progress   = 0;
}

void FFMPEGVideoExporter::createVideo(AbstractVideoExporter::ContextHandling contextHandling, std::function<void(int, int)> progress)
{
    auto width = m_parameters.at("width").toULongLong();
    auto height = m_parameters.at("height").toULongLong();

    auto fps = m_parameters.at("fps").toULongLong();
    auto viewport = glm::vec4(0, 0, width, height);
    auto length = m_parameters.at("duration").toULongLong() * fps;
    auto timeDelta = 1.f / static_cast<float>(fps);

    createAndSetupGeometry();
    createAndSetupShader();
    createAndSetupBuffer();

    m_canvas->onSaveViewport();
    m_canvas->onViewport(viewport, viewport);

    if (contextHandling == AbstractVideoExporter::ActivateContext)
    {
        m_canvas->openGLContext()->use();
    }

    if (!m_videoEncoder->initEncoding(m_parameters))
    {
        critical() << "Error in initializing video encoding.";
        return;
    }

    for (unsigned int i = 0; i < length; ++i)
    {
        m_canvas->environment()->update(timeDelta);
        m_canvas->onRender(m_fbo);
        // blitten auf default fbo zum anzeigen


        m_fbo_quad->bind(gl::GL_FRAMEBUFFER);

        gl::glViewport(
            viewport.x,
            viewport.y,
            viewport.z,
            viewport.w
        );

        gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        m_color->bindActive(0);

        m_program->use();
        m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
        m_program->release();

        m_color->unbindActive(0);

        Framebuffer::unbind(gl::GL_FRAMEBUFFER);


        m_color_quad->getImage(0, m_image->format(), m_image->type(), m_image->data());

        m_videoEncoder->putFrame(*m_image);

        m_progress = i*100/length;
        progress(i, length);
    }

    m_videoEncoder->finishEncoding();

    if (contextHandling == AbstractVideoExporter::ActivateContext)
    {
        m_canvas->openGLContext()->release();
    }

    m_canvas->onResetViewport();

    progress(1, 1);
    m_progress = 100;
}

void FFMPEGVideoExporter::onRender(ContextHandling contextHandling, globjects::Framebuffer * targetFBO, bool shouldFinalize)
{

    if (!m_initialized) initialize(contextHandling);

    auto width = m_parameters.at("width").toULongLong();
    auto height = m_parameters.at("height").toULongLong();
    auto viewport = glm::vec4(0, 0, width, height);

    // m_canvas->environment()->update(timeDelta);
    // m_canvas->environment()->update();
    m_canvas->onRender(m_fbo);

    auto destVP = m_canvas->savedDeviceViewport();

    // Fix conversion
    std::array<gl::GLint, 4> srcRect = {{viewport.x, viewport.y, viewport.z, viewport.w}};
    std::array<gl::GLint, 4> destRect = {{destVP.x, destVP.y, destVP.z, destVP.w}};

    if (!targetFBO) targetFBO = globjects::Framebuffer::defaultFBO();

    m_fbo->blit(gl::GL_COLOR_ATTACHMENT0, srcRect, targetFBO, gl::GL_COLOR_ATTACHMENT0, destRect, gl::GL_COLOR_BUFFER_BIT, gl::GL_LINEAR);
    
    m_fbo_quad->bind(gl::GL_FRAMEBUFFER);

    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    m_color->bindActive(0);

    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
    m_program->release();

    m_color->unbindActive(0);

    Framebuffer::unbind(gl::GL_FRAMEBUFFER);


    m_color_quad->getImage(0, m_image->format(), m_image->type(), m_image->data());

    m_videoEncoder->putFrame(*m_image);

    if (shouldFinalize)
    {
        finalize();
    }
}

int FFMPEGVideoExporter::progress() const
{
    return m_progress;
}

void FFMPEGVideoExporter::initialize(ContextHandling contextHandling)
{
    m_contextHandling = contextHandling;

    auto width = m_parameters.at("width").toULongLong();
    auto height = m_parameters.at("height").toULongLong();

    // auto fps = m_parameters.at("fps").toULongLong();
    auto viewport = glm::vec4(0, 0, width, height);

    createAndSetupGeometry();
    createAndSetupShader();
    createAndSetupBuffer();

    m_fbo->clearBuffer(gl::GL_COLOR, 0, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});

    m_canvas->onSaveViewport();
    m_canvas->onViewport(viewport, viewport);

    if (m_contextHandling == AbstractVideoExporter::ActivateContext)
    {
        m_canvas->openGLContext()->use();
    }

    if (!m_videoEncoder->initEncoding(m_parameters))
    {
        critical() << "Error in initializing video encoding.";
        return;
    }

    m_initialized = true;
}

void FFMPEGVideoExporter::finalize()
{
    m_videoEncoder->finishEncoding();

    if (m_contextHandling == AbstractVideoExporter::ActivateContext)
    {
        m_canvas->openGLContext()->release();
    }

    m_canvas->onResetViewport();

    m_initialized = false;
}

void FFMPEGVideoExporter::createAndSetupGeometry()
{
    static const std::array<glm::vec2, 4> raw { {
        glm::vec2( +1.f, -1.f ),
        glm::vec2( +1.f, +1.f ),
        glm::vec2( -1.f, -1.f ),
        glm::vec2( -1.f, +1.f ) } };
        
    m_fbo = new Framebuffer();
    m_color = Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depth = new Renderbuffer();
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);

    m_fbo_quad = new Framebuffer();
    m_color_quad = Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depth_quad = new Renderbuffer();
    m_fbo_quad->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color_quad);
    m_fbo_quad->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth_quad);
    
    m_vao = new VertexArray;
    auto buffer = new Buffer();
    buffer->setData(raw, gl::GL_STATIC_DRAW); //needed for some drivers
    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(buffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);
}

void FFMPEGVideoExporter::createAndSetupShader()
{
    StringTemplate * vertexShaderSource   = new StringTemplate(new StaticStringSource(s_vertexShader  ));
    StringTemplate * fragmentShaderSource = new StringTemplate(new StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    auto vertexShader   = new Shader(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    auto fragmentShader = new Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_program = new Program();
    m_program->attach(vertexShader, fragmentShader);
    m_program->setUniform("source", 0);
}

void FFMPEGVideoExporter::createAndSetupBuffer()
{
    auto width = m_parameters.at("width").toULongLong();
    auto height = m_parameters.at("height").toULongLong();

    m_image = new Image(width, height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE);

    m_color->image2D(0, m_image->format(), m_image->width(), m_image->height(), 0, m_image->format(), m_image->type(), nullptr);
    m_depth->storage(gl::GL_DEPTH_COMPONENT32, m_image->width(), m_image->height());

    m_color_quad->image2D(0, m_image->format(), m_image->width(), m_image->height(), 0, m_image->format(), m_image->type(), nullptr);
    m_depth_quad->storage(gl::GL_DEPTH_COMPONENT32, m_image->width(), m_image->height());
}
