
#pragma once


#include <string>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Canvas;
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
    *    Constructor
    *
    *  @param[in] canvas
    *    Canvas from which the screenshot is taken (must NOT be null!)
    */
    ImageExporter(Canvas * canvas);

    /**
    *  @brief
    *    Destructor
    */
    ~ImageExporter();

    /**
    *  @brief
    *    Initialize
    *
    *  @param[in] filename
    *    Name of output image file
    *  @param[in] width
    *    Width (in pixels) of output image
    *  @param[in] height
    *    Height (in pixels) of output image
    *  @param[in] renderIterations
    *    Number of render iterations
    */
    void init(const std::string & filename, int width = 0, int height = 0, int renderIterations = 1);

    /**
    *  @brief
    *    Actual call to export image creation
    *
    *  @param[in] glContextActive
    *    Indicator whether an openGLContext already is active and does not have to be activated by the ImageExporter
    */
    void save(bool glContextActive = false);


protected:
    Canvas            * m_canvas;
    Environment       * m_environment;
    AbstractGLContext * m_glContext;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Texture> m_color;
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;

    std::string m_filename;
    int         m_width;
    int         m_height;
    int         m_renderIterations;
};


} // namespace gloperate
