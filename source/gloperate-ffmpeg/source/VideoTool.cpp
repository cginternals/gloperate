
#include <gloperate-ffmpeg/VideoTool.h>

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


VideoTool::VideoTool()
{
}

VideoTool::VideoTool(const std::string & filename, RenderSurface * surface, uint fps, uint length, uint width, uint height)
: m_videoEncoder(new VideoEncoder())
// , m_fbo(new Framebuffer())
, m_fbo(nullptr)
, m_context(surface->viewerContext())
, m_surface(surface)
, m_glContext(surface->openGLContext())
, m_filename(filename)
, m_fps(fps)
, m_length(length)
, m_width(width)
, m_height(height)
, m_timeDelta(1.f / static_cast<float>(fps))
{
}

VideoTool::~VideoTool()
{
}

void VideoTool::init(const std::string & filename, gloperate::RenderSurface * surface, uint width, uint height, uint fps, uint length)
{
    m_videoEncoder = new VideoEncoder();
    // m_fbo = new Framebuffer();
    // m_fbo = Framebuffer::defaultFBO();
    m_fbo = nullptr;
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

void VideoTool::createVideo(std::function<void(int, int)> progress, bool glContextActive)
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

    // m_fbo->bind();
    m_videoEncoder->initEncoding(m_filename, m_width, m_height, m_fps);
    
    for (uint i = 0; i < length; ++i)
    {
        m_context->update(m_timeDelta);
        m_surface->onRender();

        gl::glReadPixels(0, 0, m_width, m_height, image.format(), image.type(), image.data());
    
        m_videoEncoder->putFrame(image);
        progress(i, length);
    }

    m_videoEncoder->finishEncoding();
    // m_fbo->unbind();

    if (!glContextActive)
    {
        m_glContext->release();
    }

    m_surface->onViewport(deviceViewport, virtualViewport);
}



} // namespace gloperate_ffmpeg
