
#pragma once


#include <string>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class AbstractCanvas;
class Environment;
class AbstractGLContext;
class ResourceManager;


/**
*  @brief
*    Tool to export images (screenshots) from a canvas
*/
class GLOPERATE_API ImageExporter
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
    ImageExporter();

    /**
    *  @brief
    *    Destructor
    */
    ~ImageExporter();

    /**
    *  @brief
    *    Set target screenshot configuration
    *
    *  @param[in] canvas
    *    Canvas from which the screenshot is taken (must NOT be null!)
    *  @param[in] filename
    *    Name of output image file
    *  @param[in] width
    *    Width (in pixels) of output image
    *  @param[in] height
    *    Height (in pixels) of output image
    *  @param[in] renderIterations
    *    Number of render iterations
    */
    void setTarget(AbstractCanvas * canvas, const std::string & filename, int width = 0, int height = 0, int renderIterations = 1);

    /**
    *  @brief
    *    Actual call to export image creation
    *
    *  @param[in] contextHandling
    *    Defines whether the exporter will activate and later release the OpenGL context
    */
    void save(ContextHandling contextHandling = ActivateContext);


protected:
    // Configuration
    AbstractCanvas * m_canvas;
    std::string      m_filename;
    int              m_width;
    int              m_height;
    int              m_renderIterations;

    // OpenGl objects
    globjects::ref_ptr<globjects::Framebuffer>  m_fbo;
    globjects::ref_ptr<globjects::Texture>      m_color;
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;
};


} // namespace gloperate
