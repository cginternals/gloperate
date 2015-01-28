#include <gloperate/tools/ScreenshotTool.h>

#include <cassert>

#include <glbinding/gl/enum.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>

#include <gloperate/resources/ResourceManager.h>


namespace gloperate
{

ScreenshotTool::ScreenshotTool(Painter * painter, ResourceManager & resourceManager)
    : m_painter(painter)
    , m_resourceManager(resourceManager)
    , m_viewportCapability(painter->getCapability<AbstractViewportCapability>())
    , m_framebufferCapability(painter->getCapability<AbstractTargetFramebufferCapability>())
{
    assert(isApplicableTo(painter));
}

bool ScreenshotTool::isApplicableTo(Painter * painter)
{
    return painter->getCapability<AbstractViewportCapability>() != nullptr
        && painter->getCapability<AbstractTargetFramebufferCapability>() != nullptr;
}

void ScreenshotTool::initialize()
{
    m_fbo = new globjects::Framebuffer();
    m_color = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depth = new globjects::Renderbuffer();

    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);
}

void ScreenshotTool::save(const std::string & filename)
{
    m_color->image2D(0, gl::GL_RGBA, m_viewportCapability->width(), m_viewportCapability->height(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    // [TODO] Check for availability of depth format
    m_depth->storage(gl::GL_DEPTH_COMPONENT32, m_viewportCapability->width(), m_viewportCapability->height());

    globjects::Framebuffer * oldFbo = m_framebufferCapability->framebuffer();
    m_framebufferCapability->setFramebuffer(m_fbo);

    m_painter->paint();

    // [TODO] handle filename
    m_resourceManager.store<globjects::Texture>(filename, m_color);

    m_framebufferCapability->setFramebuffer(oldFbo);
}

void ScreenshotTool::save(const std::string &filename, const int width, const int height)
{
    int oldWidth = m_viewportCapability->width();
    int oldHeight = m_viewportCapability->height();

    m_viewportCapability->setViewport(m_viewportCapability->x(), m_viewportCapability->y(), width, height);
    save(filename);
    m_viewportCapability->setViewport(m_viewportCapability->x(), m_viewportCapability->y(), oldWidth, oldHeight);
}

} // namespace gloperate
