
#include <gloperate-ffmpeg/VideoTool.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/RenderSurface.h>
#include <gloperate/viewer/AbstractGLContext.h>
// #include <gloperate/pipeline/Stage.h>

#include <globjects/base/baselogging.h>

#include <glbinding/gl/gl.h>


using namespace globjects;
using namespace gloperate;


namespace gloperate_ffmpeg
{


VideoTool::VideoTool(const std::string & filename, RenderSurface * surface, uint fps, uint length, uint width, uint height)
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
{
    auto vp = glm::ivec4(0, 0, m_width, m_height);

    m_surface->onViewport(vp, vp);
}

VideoTool::~VideoTool()
{
}

void VideoTool::createVideo(std::function<void(int, int)> progress)
{
    auto length = m_length * m_fps;
    
    Image image(m_width, m_height, Image::Format::RGB24);

    m_glContext->use();
    m_videoEncoder->initEncoding(m_filename);
    
    for (uint i = 0; i < length; ++i)
    {
        m_context->update(m_timeDelta);
        m_surface->onRender();

        gl::glReadPixels(0, 0, m_width, m_height, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, image.data());
    
        m_videoEncoder->putFrame(image);
        progress(i, length);
    }

    m_videoEncoder->finishEncoding();
    m_glContext->release();
}



} // namespace gloperate_ffmpeg
