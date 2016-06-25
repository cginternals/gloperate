
#pragma once


#include <string>
#include <functional>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Canvas;


/**
*  @brief
*    Abstract class for video exporter classes which render from a canvas to a video file
*/
class GLOPERATE_API AbstractVideoExporter
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractVideoExporter();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractVideoExporter();

    /**
    *  @brief
    *    Initialize
    *
    *  @param[in] filename
    *    Name of output video file
    *  @param[in] canvas
    *    Canvas that will be rendered into a video
    *  @param[in] width
    *    Width (in pixels) of output video
    *  @param[in] height
    *    Height (in pixels) of output video
    *  @param[in] fps
    *    Frames per second of output video
    *  @param[in] length
    *    Length (in seconds) of output video
    */
    virtual void init(const std::string & filename, Canvas * canvas, uint width, uint height, uint fps, uint length) = 0;

    /**
    *  @brief
    *    Actual call to start video creation
    *
    *  @param[in] progress
    *    Progress callback function
    *  @param[in] glContextActive
    *    Indicator whether an openGLContext already is active and does not have to be activated by the FFMPEGVideoExporter
    */
    virtual void createVideo(std::function<void(int, int)> progress, bool glContextActive) = 0;

    /**
    *  @brief
    *    Get progress in percent, ranging from 0 (no progress) to 100 (complete)
    *
    *  @return
    *    Progress in percent
    */
    virtual int progress() const;
};


} // namespace gloperate
