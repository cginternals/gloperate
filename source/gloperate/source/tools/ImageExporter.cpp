
#include <gloperate/tools/ImageExporter.h>

#include <cassert>

#include <glbinding/gl/enum.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>
#include <gloperate/resources/ResourceManager.h>


namespace gloperate
{


ImageExporter::ImageExporter(Painter * painter, ResourceManager & resourceManager)
    : m_painter(painter)
    , m_resourceManager(resourceManager)
    , m_viewportCapability(painter->getCapability<AbstractViewportCapability>())
    , m_framebufferCapability(painter->getCapability<AbstractTargetFramebufferCapability>())
{
    assert(isApplicableTo(painter));
}

bool ImageExporter::isApplicableTo(Painter * painter)
{
    return painter->getCapability<AbstractViewportCapability>() != nullptr
        && painter->getCapability<AbstractTargetFramebufferCapability>() != nullptr;
}

void ImageExporter::initialize()
{
    m_fbo = new globjects::Framebuffer();
    m_color = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_depth = new globjects::Renderbuffer();

    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_color);
    m_fbo->attachRenderBuffer(gl::GL_DEPTH_ATTACHMENT, m_depth);
}

void ImageExporter::save(const std::string & filename, const int & width, const int & height, const int & renderIterations)
{
	const int oldWidth{ m_viewportCapability->width() }, oldHeight{ m_viewportCapability->height() }, oldX{ m_viewportCapability->x() }, oldY{ m_viewportCapability->y() };
	if (width > 0 && height > 0)
		m_viewportCapability->setViewport(0, 0, width, height);

    m_color->image2D(0, gl::GL_RGBA, m_viewportCapability->width(), m_viewportCapability->height(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    // [TODO] Check for availability of depth format
    m_depth->storage(gl::GL_DEPTH_COMPONENT32, m_viewportCapability->width(), m_viewportCapability->height());

    globjects::Framebuffer * oldFbo = m_framebufferCapability->framebuffer();
    m_framebufferCapability->setFramebuffer(m_fbo);

	for (int i = 0; i < renderIterations; i++)
		m_painter->paint();

    // [TODO] handle filename
    m_resourceManager.store<globjects::Texture>(filename, m_color);

    m_framebufferCapability->setFramebuffer(oldFbo);
	if (width > 0 && height > 0)
		m_viewportCapability->setViewport(oldX, oldY, oldWidth, oldHeight);
}


} // namespace gloperate
