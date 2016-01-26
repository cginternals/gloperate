
#pragma once


#include <string>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Painter;
class ResourceManager;
class AbstractViewportCapability;
class AbstractTargetFramebufferCapability;


/**
*  @brief
*    Tool to export images (screenshots) from a painter
*/
class GLOPERATE_API ImageExporter
{
public:
	ImageExporter(Painter * painter, ResourceManager & resourceManager);

    static bool isApplicableTo(Painter * painter);

    void initialize();

	void save(const std::string & filename, const int & width = 0, const int & height = 0, const int & renderIterations = 1);


protected:
    Painter * m_painter;
    ResourceManager & m_resourceManager;
    AbstractViewportCapability * m_viewportCapability;
    AbstractTargetFramebufferCapability * m_framebufferCapability;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Texture> m_color;
    globjects::ref_ptr<globjects::Renderbuffer> m_depth;
};


} // namespace gloperate
