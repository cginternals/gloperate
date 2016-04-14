
#pragma once


#include <glm/glm.hpp>

#include <string>
#include <functional>

#include <globjects/base/ref_ptr.h>

#include <gloperate/output/AbstractVideoTool.h>

#include <gloperate-ffmpeg/VideoEncoder.h>

#include <gloperate-ffmpeg/gloperate-ffmpeg_api.h>


namespace gloperate {
    class ViewerContext;
    class RenderSurface;
    class AbstractGLContext;
}

namespace globjects {
    class Framebuffer;
}


namespace gloperate_ffmpeg
{


/**
*  @brief
*    A tool which renders a given Stage into an output video file.
*/
class GLOPERATE_FFMPEG_API VideoTool : public gloperate::AbstractVideoTool
{
public:
    /**
    *  @brief
    *    Constructor
    */
    VideoTool();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] filename
    *    Name of output video file
    *  @param[in] surface
    *    The surface that will be rendered into a video
    *  @param[in] fps
    *    Frames per second of output video
    *  @param[in] length
    *    Length (in seconds) of output video
    *  @param[in] width
    *    Width (in pixels) of output video
    *  @param[in] height
    *    Height (in pixels) of output video
    */
    VideoTool(const std::string & filename,
              gloperate::RenderSurface * surface,
              uint fps,
              uint length,
              uint width,
              uint height);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~VideoTool();

    virtual void init(const std::string & filename, gloperate::RenderSurface * surface, uint fps, uint length, uint width, uint height) override;

    /**
    *  @brief
    *    Actual call to start video creation
    *
    *  @param[in] progress
    *    Progress callback function
    */
    virtual void createVideo(std::function<void(int, int)> progress, bool glContextActive = false) override;


protected:
    globjects::ref_ptr<VideoEncoder>             m_videoEncoder;
    globjects::ref_ptr<globjects::Framebuffer>   m_fbo;
    gloperate::ViewerContext                   * m_context;
    gloperate::RenderSurface                   * m_surface;
    gloperate::AbstractGLContext               * m_glContext;
    std::string                                  m_filename;
    uint                                         m_fps;
    uint                                         m_length;
    uint                                         m_width;
    uint                                         m_height;
    float                                        m_timeDelta;
};


} // namespace gloperate_ffmpeg
