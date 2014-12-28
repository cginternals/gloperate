#include <gloperate/tools/DepthExtractor.h>

#include <limits>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>

#include <gloperate/base/RenderTarget.h>

namespace gloperate
{

DepthExtractor::DepthExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   m_viewportCapability(viewportCapability)
,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
}

DepthExtractor::~DepthExtractor()
{
}

float DepthExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    if (!m_typedRenderTargetCapability || !m_viewportCapability)
    {
        return 1;
    }

    if (!m_typedRenderTargetCapability->hasRenderTarget(RenderTargetType::Depth))
        return 1.f;

    const gloperate::RenderTarget & depthTarget = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Depth);

    const gl::GLint x = static_cast<gl::GLint>(m_viewportCapability->x());
    const gl::GLint y = static_cast<gl::GLint>(m_viewportCapability->y());
    const gl::GLint w = static_cast<gl::GLint>(m_viewportCapability->width()); 
    const gl::GLint h = static_cast<gl::GLint>(m_viewportCapability->height());

    if (x > windowCoordinates.x || windowCoordinates.x >= w || y > windowCoordinates.y || windowCoordinates.y >= h)
        return 1.f;

    depthTarget.framebuffer()->bind(gl::GL_READ_FRAMEBUFFER);
    gl::glBindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

    if (depthTarget.attachment() != gl::GL_DEPTH_ATTACHMENT)
        gl::glReadBuffer(depthTarget.attachment()); // glReadBuffer does not accept GL_DEPTH_ATTACHMENT and causes an error

    gl::GLfloat z;

    gl::glReadPixels(windowCoordinates.x, h - windowCoordinates.y, 1, 1, depthTarget.format(), gl::GL_FLOAT, reinterpret_cast<void*>(&z));

    gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);

    return z;
}

bool DepthExtractor::isValidDepth(const float depth)
{
    return depth < (1.f - std::numeric_limits<float>::epsilon());
}

} // namespace gloperate
