
#pragma once


#include <glm/glm.hpp>

#include <string>
#include <functional>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>
#include <globjects/VertexArray.h>
#include <globjects/Program.h>

#include <gloperate/tools/AbstractVideoExporter.h>

#include <gloperate/gloperate-version.h>

#include "FFMPEGVideoEncoder.h"


namespace gloperate {
    class Environment;
    class Canvas;
    class AbstractGLContext;
}


/**
*  @brief
*    A tool which renders a given Stage into an output video file.
*/
class FFMPEGVideoExporter : public gloperate::AbstractVideoExporter
{
public:
CPPEXPOSE_DECLARE_COMPONENT(
    FFMPEGVideoExporter, gloperate::AbstractVideoExporter
  , "" // Tags
  , "" // Icon
  , "" // Annotations
  , "Export canvas to video using FFMPEG"
  , GLOPERATE_AUTHOR_ORGANIZATION
  , "v1.0.0"
)

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
    FFMPEGVideoExporter(const std::string & filename,
              gloperate::Canvas * canvas,
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
    virtual void init(const std::string & filename, gloperate::Canvas * canvas, uint width, uint height, uint fps, uint length) override;

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
    void createAndSetupGeometry();
    void createAndSetupShader();


protected:
    FFMPEGVideoEncoder                         * m_videoEncoder;
    gloperate::Environment                     * m_environment;
    gloperate::Canvas                          * m_canvas;
    gloperate::AbstractGLContext               * m_glContext;

    globjects::ref_ptr<globjects::Framebuffer>   m_fbo;
    globjects::ref_ptr<globjects::Texture>       m_color;
    globjects::ref_ptr<globjects::Renderbuffer>  m_depth;
    globjects::ref_ptr<globjects::Framebuffer>   m_fbo_quad;
    globjects::ref_ptr<globjects::Texture>       m_color_quad;
    globjects::ref_ptr<globjects::Renderbuffer>  m_depth_quad;
    globjects::ref_ptr<globjects::VertexArray>   m_vao;
    globjects::ref_ptr<globjects::Program>       m_program;

    std::string                                  m_filename;
    uint                                         m_fps;
    uint                                         m_length;
    uint                                         m_width;
    uint                                         m_height;
    float                                        m_timeDelta;
    int                                          m_progress;
};
