
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
    class AbstractCanvas;
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
    *    Destructor
    */
    virtual ~FFMPEGVideoExporter();

    // Virtual AbstractVideoExporter interface
    virtual void setTarget(gloperate::AbstractCanvas * canvas, const std::string & filename, unsigned int width, unsigned int height, unsigned int fps, unsigned int length) override;
    virtual void createVideo(AbstractVideoExporter::ContextHandling contextHandling, std::function<void(int, int)> progress) override;
    virtual int progress() const override;


protected:
    void createAndSetupGeometry();
    void createAndSetupShader();


protected:
    FFMPEGVideoEncoder                         * m_videoEncoder;
    gloperate::AbstractCanvas                  * m_canvas;

    globjects::ref_ptr<globjects::Framebuffer>   m_fbo;
    globjects::ref_ptr<globjects::Texture>       m_color;
    globjects::ref_ptr<globjects::Renderbuffer>  m_depth;
    globjects::ref_ptr<globjects::Framebuffer>   m_fbo_quad;
    globjects::ref_ptr<globjects::Texture>       m_color_quad;
    globjects::ref_ptr<globjects::Renderbuffer>  m_depth_quad;
    globjects::ref_ptr<globjects::VertexArray>   m_vao;
    globjects::ref_ptr<globjects::Program>       m_program;

    std::string                                  m_filename;
    unsigned int                                 m_fps;
    unsigned int                                 m_length;
    unsigned int                                 m_width;
    unsigned int                                 m_height;
    float                                        m_timeDelta;
    int                                          m_progress;
};
