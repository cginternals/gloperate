
#include "FFMPEGVideoExporter.h"

#include <glm/vec2.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>
#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <gloperate/base/AbstractGLContext.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Shader.h>
#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Program.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/RenderSurface.h>

#include <gloperate/gloperate.h>


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
: m_progress(0)
{
}

FFMPEGVideoExporter::FFMPEGVideoExporter(const std::string & filename, RenderSurface * surface, uint fps, uint length, uint width, uint height)
: m_videoEncoder(new FFMPEGVideoEncoder())
, m_context(surface->viewerContext())
, m_surface(surface)
, m_glContext(surface->openGLContext())
, m_filename(filename)
, m_fps(fps)
, m_length(length)
, m_width(width)
, m_height(height)
, m_timeDelta(1.f / static_cast<float>(fps))
, m_progress(0)
{
}

FFMPEGVideoExporter::~FFMPEGVideoExporter()
{
    delete m_videoEncoder;
}

void FFMPEGVideoExporter::init(const std::string & filename, gloperate::RenderSurface * surface, uint width, uint height, uint fps, uint length)
{
    m_videoEncoder = new FFMPEGVideoEncoder();
    m_context = surface->viewerContext();
    m_surface = surface;
    m_glContext = surface->openGLContext();
    m_filename = filename;
    m_fps = fps;
    m_length = length;
    m_width = width;
    m_height = height;
    m_timeDelta = 1.f/static_cast<float>(fps);
    m_progress = 0;
}

void FFMPEGVideoExporter::createVideo(std::function<void(int, int)> progress, bool glContextActive)
{
    static const std::array<glm::vec2, 4> raw { {
        glm::vec2( +1.f, -1.f ),
        glm::vec2( +1.f, +1.f ),
        glm::vec2( -1.f, -1.f ),
        glm::vec2( -1.f, +1.f ) } };

    auto deviceViewport = m_surface->deviceViewport();
    auto virtualViewport = m_surface->virtualViewport();
    auto vp = glm::vec4(0, 0, m_width, m_height);
    auto length = m_length * m_fps;

    auto fbo = new Framebuffer();
    auto color = Texture::createDefault(gl::GL_TEXTURE_2D);
    auto depth = new Renderbuffer();
    fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, color);
    fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, depth);


    auto fbo_quad = new Framebuffer();
    auto color_quad = Texture::createDefault(gl::GL_TEXTURE_2D);
    auto depth_quad = new Renderbuffer();
    fbo_quad->attachTexture(gl::GL_COLOR_ATTACHMENT0, color_quad);
    fbo_quad->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, depth_quad);
    
    auto vao = new VertexArray;
    auto buffer = new Buffer();
    buffer->setData(raw, gl::GL_STATIC_DRAW); //needed for some drivers
    auto binding = vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(buffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    vao->enable(0);

    StringTemplate * vertexShaderSource   = new StringTemplate(new StaticStringSource(s_vertexShader  ));
    StringTemplate * fragmentShaderSource = new StringTemplate(new StaticStringSource(s_fragmentShader));

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    auto vertexShader   = new Shader(gl::GL_VERTEX_SHADER,   vertexShaderSource);
    auto fragmentShader = new Shader(gl::GL_FRAGMENT_SHADER, fragmentShaderSource);
    auto program = new Program();
    program->attach(vertexShader, fragmentShader);
    program->setUniform("source", 0);


    m_surface->onViewport(vp, vp);

    Image image(m_width, m_height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE);

    if (!glContextActive)
    {
        m_glContext->use();
    }

    m_videoEncoder->initEncoding(m_filename, m_width, m_height, m_fps);

    for (uint i = 0; i < length; ++i)
    {
        color->image2D(0, image.format(), image.width(), image.height(), 0, image.format(), image.type(), nullptr);
        depth->storage(gl::GL_DEPTH_COMPONENT32, image.width(), image.height());

        color_quad->image2D(0, image.format(), image.width(), image.height(), 0, image.format(), image.type(), nullptr);
        depth_quad->storage(gl::GL_DEPTH_COMPONENT32, image.width(), image.height());

        m_context->update(m_timeDelta);
        m_surface->onRender(fbo);


        fbo_quad->bind(gl::GL_FRAMEBUFFER);

        // Update viewport
        gl::glViewport(
            vp.x,
            vp.y,
            vp.z,
            vp.w
        );

        // Clear background
        gl::glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

        color->bindActive(0);

        program->use();
        vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
        program->release();

        color->unbindActive(0);

        Framebuffer::unbind(gl::GL_FRAMEBUFFER);


        color_quad->getImage(0, image.format(), image.type(), image.data());

        m_videoEncoder->putFrame(image);

        m_progress = i*100/length;
        progress(i, length);
    }

    m_videoEncoder->finishEncoding();

    if (!glContextActive)
    {
        m_glContext->release();
    }

    m_surface->onViewport(deviceViewport, virtualViewport);

    progress(1, 1);
    m_progress = 100;
}

int FFMPEGVideoExporter::progress() const
{
    return m_progress;
}
