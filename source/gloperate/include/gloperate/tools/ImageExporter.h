
#pragma once


#include <string>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class RenderSurface;
class ViewerContext;
class AbstractGLContext;
class ResourceManager;


/**
*  @brief
*    Tool to export images (screenshots) from a painter
*/
class GLOPERATE_API ImageExporter
{
public:
    ImageExporter(RenderSurface * surface);

    void init(const std::string & filename, int width = 0, int height = 0, int renderIterations = 1);

    void save(bool glContextActive = false);


protected:
    RenderSurface                   * m_surface;
    ViewerContext                   * m_context;
    AbstractGLContext               * m_glContext;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Texture> m_color;
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;

    std::string m_filename;
    int         m_width;
    int         m_height;
    int         m_renderIterations;
};


} // namespace gloperate