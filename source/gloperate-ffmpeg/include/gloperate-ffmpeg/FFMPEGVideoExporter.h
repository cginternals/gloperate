
#pragma once


#include <glm/glm.hpp>

#include <string>
#include <functional>

#include <globjects/base/ref_ptr.h>

#include <gloperate/tools/AbstractVideoExporter.h>

#include <gloperate-ffmpeg/VideoEncoder.h>

#include <gloperate-ffmpeg/gloperate-ffmpeg_api.h>


namespace gloperate {
    class ViewerContext;
    class RenderSurface;
    class AbstractGLContext;
}


namespace gloperate_ffmpeg
{


/**
*  @brief
*    A tool which renders a given Stage into an output video file.
*/
class GLOPERATE_FFMPEG_API FFMPEGVideoExporter : public gloperate::AbstractVideoExporter
{
public:
    /**
    *  @brief
    *    Constructor
    */
    FFMPEGVideoExporter();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] filename
    *    Name of output video file
    *  @param[in] surface
    *    The surface that will be rendered into a video
    *  @param[in] width
    *    Width (in pixels) of output video
    *  @param[in] height
    *    Height (in pixels) of output video
    *  @param[in] fps
    *    Frames per second of output video
    *  @param[in] length
    *    Length (in seconds) of output video
    */
    FFMPEGVideoExporter(const std::string & filename,
              gloperate::RenderSurface * surface,
              uint fps,
              uint length,
              uint width,
              uint height);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~FFMPEGVideoExporter();

    /**
    *  @brief
    *    Initialize
    *
    *  @param[in] filename
    *    Name of output video file
    *  @param[in] surface
    *    The surface that will be rendered into a video
    *  @param[in] width
    *    Width (in pixels) of output video
    *  @param[in] height
    *    Height (in pixels) of output video
    *  @param[in] fps
    *    Frames per second of output video
    *  @param[in] length
    *    Length (in seconds) of output video
    */
    virtual void init(const std::string & filename, gloperate::RenderSurface * surface, uint width, uint height, uint fps, uint length) override;

    /**
    *  @brief
    *    Actual call to start video creation
    *
    *  @param[in] progress
    *    Progress callback function
    *  @param[in] glContextActive
    *    Indicator whether an openGLContext already is active and does not have to be activated by the FFMPEGVideoExporter
    */
    virtual void createVideo(std::function<void(int, int)> progress, bool glContextActive = false) override;

    /**
    *  @brief
    *    Get progress in percent, ranging from 0 (no progress) to 100 (complete)
    *
    *  @return
    *    Progress in percent
    */
    virtual int progress() const override;


protected:
    VideoEncoder                               * m_videoEncoder;
    gloperate::ViewerContext                   * m_context;
    gloperate::RenderSurface                   * m_surface;
    gloperate::AbstractGLContext               * m_glContext;
    std::string                                  m_filename;
    uint                                         m_fps;
    uint                                         m_length;
    uint                                         m_width;
    uint                                         m_height;
    float                                        m_timeDelta;
    int                                          m_progress;
};


} // namespace gloperate_ffmpeg
