
#include <gloperate-ffmpeg/FFMPEGVideoExporter.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/RenderSurface.h>
#include <gloperate/viewer/AbstractGLContext.h>

#include <globjects/base/baselogging.h>
#include <globjects/Framebuffer.h>

#include <glbinding/gl/gl.h>


using namespace globjects;
using namespace gloperate;


namespace gloperate_ffmpeg
{


FFMPEGVideoExporter::FFMPEGVideoExporter()
: m_progress(0)
{
}

FFMPEGVideoExporter::FFMPEGVideoExporter(const std::string & filename, RenderSurface * surface, uint fps, uint length, uint width, uint height)
: m_videoEncoder(new VideoEncoder())
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
}

void FFMPEGVideoExporter::init(const std::string & filename, gloperate::RenderSurface * surface, uint width, uint height, uint fps, uint length)
{
    m_videoEncoder = new VideoEncoder();
    m_context = surface->viewerContext();
    m_surface = surface;
    m_glContext = surface->openGLContext();
    m_filename = filename;
    m_fps = fps;
    m_length = length;
    m_width = width;
    m_height = height;
    m_timeDelta = 1.f/static_cast<float>(fps);
}

void FFMPEGVideoExporter::createVideo(std::function<void(int, int)> progress, bool glContextActive)
{
    auto deviceViewport = m_surface->deviceViewport();
    auto virtualViewport = m_surface->virtualViewport();
    auto vp = glm::vec4(0, 0, m_width, m_height);
    auto length = m_length * m_fps;

    m_surface->onViewport(vp, vp);

    Image image(m_width, m_height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE);

    if (!glContextActive)
    {
        m_glContext->use();
    }

    m_videoEncoder->initEncoding(m_filename, m_width, m_height, m_fps);
    
    for (uint i = 0; i < length; ++i)
    {
        m_context->update(m_timeDelta);
        m_surface->onRender();

        gl::glReadPixels(0, 0, m_width, m_height, image.format(), image.type(), image.data());
    
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


} // namespace gloperate_ffmpeg
