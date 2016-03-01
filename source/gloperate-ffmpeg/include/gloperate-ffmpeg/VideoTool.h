
#pragma once


#include <glm/glm.hpp>

#include <string>
#include <functional>

#include <globjects/base/ref_ptr.h>

#include <gloperate-ffmpeg/VideoEncoder.h>

#include <gloperate-ffmpeg/gloperate-ffmpeg_api.h>


namespace gloperate {
    class Stage;
    class ViewerContext;
    class RenderSurface;
    class AbstractGLContext;
}


namespace gloperate_ffmpeg
{


/**
*  @brief
*    
*/
class GLOPERATE_FFMPEG_API VideoTool
{
public:
    /**
    *  @brief
    *    Constructor
    */
    VideoTool(const std::string & filename, gloperate::Stage * stage, gloperate::AbstractGLContext * glContext, uint fps, uint length, uint width, uint height);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~VideoTool();

    void createVideo(std::function<void(int, int)> progress);


protected:
    globjects::ref_ptr<VideoEncoder>  m_videoEncoder;
    gloperate::ViewerContext        * m_context;
    gloperate::Stage                * m_stage;
    gloperate::RenderSurface        * m_surface;
    gloperate::AbstractGLContext    * m_glContext;
    std::string                       m_filename;
    uint                              m_fps;
    uint                              m_length;
    uint                              m_width;
    uint                              m_height;
    float                             m_timeDelta;
};


} // namespace gloperate_ffmpeg
