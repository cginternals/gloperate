
#pragma once


#include <glm/glm.hpp>

#include <string>
#include <functional>
#include <memory>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Texture.h>
#include <globjects/Renderbuffer.h>
#include <globjects/VertexArray.h>
#include <globjects/Program.h>

#include <gloperate/tools/AbstractVideoExporter.h>

#include <gloperate/gloperate-version.h>

#include "FFMPEGVideoEncoder.h"


namespace gloperate {
    class Canvas;
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
    virtual void setTarget(gloperate::Canvas * canvas, const cppexpose::VariantMap & parameters) override;
    virtual void createVideo(AbstractVideoExporter::ContextHandling contextHandling, std::function<void(int, int)> progress) override;
    virtual void onRender(ContextHandling contextHandling, globjects::Framebuffer * targetFBO, bool shouldFinalize = false) override;
    virtual int progress() const override;


protected:
    void initialize(ContextHandling contextHandling);
    void finalize();
    void createAndSetupGeometry();
    void createAndSetupShader();
    void createAndSetupBuffer();


protected:
    FFMPEGVideoEncoder                     * m_videoEncoder;
    gloperate::Canvas                      * m_canvas;
    gloperate::Image                       * m_image;

    std::unique_ptr<globjects::Framebuffer>  m_fbo;
    std::unique_ptr<globjects::Texture>      m_color;
    std::unique_ptr<globjects::Renderbuffer> m_depth;
    std::unique_ptr<globjects::Framebuffer>  m_fbo_quad;
    std::unique_ptr<globjects::Texture>      m_color_quad;
    std::unique_ptr<globjects::Renderbuffer> m_depth_quad;
    std::unique_ptr<globjects::VertexArray>  m_vao;
    std::unique_ptr<globjects::Buffer>       m_buffer;
    std::unique_ptr<globjects::Program>      m_program;

    cppexpose::VariantMap                    m_parameters;

    int                                      m_progress;
    bool                                     m_initialized;
    AbstractVideoExporter::ContextHandling   m_contextHandling;

    glm::vec4                                m_savedViewport;
};
