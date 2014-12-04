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


class GLOPERATE_API ScreenshotTool
{
public:
    ScreenshotTool(Painter * painter, ResourceManager & resourceManager);

    static bool isApplicableTo(Painter * painter);

    void initialize();

    void save(const std::string & filename);

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
