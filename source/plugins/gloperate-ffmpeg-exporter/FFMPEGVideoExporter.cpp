
#include "FFMPEGVideoExporter.h"

#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>
#include <globjects/base/File.h>
#include <globjects/base/StringTemplate.h>
#include <gloperate/base/AbstractGLContext.h>
#include <globjects/Shader.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/RenderSurface.h>

#include <gloperate/gloperate.h>


using namespace globjects;
using namespace gloperate;


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
    auto deviceViewport = m_surface->deviceViewport();
    auto virtualViewport = m_surface->virtualViewport();
    auto vp = glm::vec4(0, 0, m_width, m_height);
    auto length = m_length * m_fps;

    m_fbo = new globjects::Framebuffer();
    m_color = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depth = new globjects::Renderbuffer();

    m_program = new globjects::Program();
    m_program->attach(new globjects::Shader(gl::GL_VERTEX_SHADER, new globjects::StringTemplate(new globjects::File(gloperate::dataPath() + "/gloperate/shaders/Exporter/FlipVertically.vert"))));

    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);

    m_surface->onViewport(vp, vp);

    Image image(m_width, m_height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE);

    if (!glContextActive)
    {
        m_glContext->use();
    }

    // m_program->use();

    m_videoEncoder->initEncoding(m_filename, m_width, m_height, m_fps);

    for (uint i = 0; i < length; ++i)
    {
        m_color->image2D(0, image.format(), image.width(), image.height(), 0, image.format(), image.type(), nullptr);
        m_depth->storage(gl::GL_DEPTH_COMPONENT32, image.width(), image.height());

        m_context->update(m_timeDelta);
        m_surface->onRender(m_fbo);

        m_color->getImage(0, image.format(), image.type(), image.data());

        m_videoEncoder->putFrame(image);

        m_progress = i*100/length;
        progress(i, length);
    }

    m_videoEncoder->finishEncoding();

    // m_program->release();

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
