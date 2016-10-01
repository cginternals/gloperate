
#pragma once


#include <string>
#include <functional>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class AbstractCanvas;


/**
*  @brief
*    Abstract class for video exporter classes which render from a canvas to a video file
*/
class GLOPERATE_API AbstractVideoExporter
{
public:
    /**
    *  @brief
    *    OpenGL context handling
    */
    enum ContextHandling
    {
        IgnoreContext,  ///< The OpenGL context is already active, no changes will be made
        ActivateContext ///< The OpenGL context will be activated and released by the function
    };


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
    *    Set target video export configuration
    *
    *  @param[in] canvas
    *    Canvas that will be rendered into a video
    *  @param[in] filename
    *    Name of output video file
    *  @param[in] width
    *    Width (in pixels) of output video
    *  @param[in] height
    *    Height (in pixels) of output video
    *  @param[in] fps
    *    Frames per second of output video
    *  @param[in] length
    *    Length (in seconds) of output video
    */
    virtual void setTarget(AbstractCanvas * canvas, const std::string & filename, unsigned int width, unsigned int height, unsigned int fps, unsigned int length) = 0;

    /**
    *  @brief
    *    Actual call to start video creation
    *
    *  @param[in] contextHandling
    *    Defines whether the exporter will activate and later release the OpenGL context
    *  @param[in] progress
    *    Progress callback function
    */
    virtual void createVideo(ContextHandling contextHandling, std::function<void(int, int)> progress) = 0;

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
