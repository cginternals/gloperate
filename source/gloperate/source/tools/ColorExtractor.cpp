#include <gloperate/tools/ColorExtractor.h>

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>

#include <gloperate/base/RenderTarget.h>

namespace gloperate
{

ColorExtractor::ColorExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   m_viewportCapability(viewportCapability)
,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
}

ColorExtractor::~ColorExtractor()
{
}

glm::vec4 ColorExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    if (!m_typedRenderTargetCapability || !m_viewportCapability)
    {
        return glm::vec4(0.0);
    }

    if (!m_typedRenderTargetCapability->hasRenderTarget(RenderTargetType::Color))
        return glm::vec4(0.0);

    const gloperate::RenderTarget & colorTarget = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Color);

    const gl::GLint x = static_cast<gl::GLint>(m_viewportCapability->x());
    const gl::GLint y = static_cast<gl::GLint>(m_viewportCapability->y());
    const gl::GLint w = static_cast<gl::GLint>(m_viewportCapability->width());
    const gl::GLint h = static_cast<gl::GLint>(m_viewportCapability->height());

    if (x > windowCoordinates.x || windowCoordinates.x >= w || y > windowCoordinates.y || windowCoordinates.y >= h)
        return glm::vec4(0.0);

    colorTarget.framebuffer()->bind(gl::GL_READ_FRAMEBUFFER);
    gl::glBindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

    if (colorTarget.attachment() != gl::GL_DEPTH_ATTACHMENT)
        gl::glReadBuffer(colorTarget.attachment()); // glReadBuffer does not accept GL_DEPTH_ATTACHMENT and causes an error

    glm::vec4 color;

    gl::glReadPixels(windowCoordinates.x, h - windowCoordinates.y, 1, 1, colorTarget.format(), gl::GL_FLOAT, reinterpret_cast<void*>(glm::value_ptr(color)));

    gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);

    return color;
}

} // namespace gloperate
