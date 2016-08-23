
#pragma once


#include <string>
#include <functional>

#include <globjects/Framebuffer.h>

#include <cppexpose/variant/Variant.h>

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
    *  @param[in] parameters
    *    Parameters for video exporting
    */
    virtual void setTarget(AbstractCanvas * canvas, const cppexpose::VariantMap & parameters) = 0;

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
    *    onRender call for asynchronous video export per frame
    *
    *  @param[in] contextHandling
    *    Defines whether the exporter will activate and later release the OpenGL context
    *  @param[in] targetFBO
    *    Target FBO on which should be rendered in addition to the video FBO
    */
    virtual void onRender(ContextHandling contextHandling, globjects::Framebuffer * targetFBO) = 0;

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
