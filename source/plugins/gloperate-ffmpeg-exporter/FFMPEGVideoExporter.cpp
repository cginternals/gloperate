
#include "FFMPEGVideoExporter.h"

#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>
#include <gloperate/base/AbstractGLContext.h>
#include <globjects/Framebuffer.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/RenderSurface.h>


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
}

void FFMPEGVideoExporter::createVideo(std::function<void(int, int)> progress, bool glContextActive)
{
    auto deviceViewport = m_surface->deviceViewport();
    auto virtualViewport = m_surface->virtualViewport();
    auto vp = glm::vec4(0, 0, m_width, m_height);
    auto length = m_length * m_fps;

    m_surface->onViewport(vp, vp);

    Image image(m_width, m_height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE);
    int bytesPerRow = image.width() * image.bytes() * image.channels();
    char * tempData = new char[bytesPerRow * image.height()];

    if (!glContextActive)
    {
        m_glContext->use();
    }

    m_videoEncoder->initEncoding(m_filename, m_width, m_height, m_fps);
    
    for (uint i = 0; i < length; ++i)
    {
        m_context->update(m_timeDelta);
        m_surface->onRender();

        gl::glReadPixels(0, 0, m_width, m_height, image.format(), image.type(), tempData);

        // Flip image vertically
        for (int y = 0; y < image.height(); ++y)
        {
            memcpy(image.data() + (y * bytesPerRow),
                tempData + ((image.height() - y) * bytesPerRow),
                bytesPerRow);
        }

        m_videoEncoder->putFrame(image);

        m_progress = i*100/length;
        progress(i, length);
    }

    m_videoEncoder->finishEncoding();

    if (!glContextActive)
    {
        m_glContext->release();
    }

    delete [] tempData;

    m_surface->onViewport(deviceViewport, virtualViewport);

    progress(1, 1);
    m_progress = 100;
}

int FFMPEGVideoExporter::progress() const
{
    return m_progress;
}
