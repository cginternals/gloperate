
#include <gloperate/painter/TypedRenderTargetCapability.h>

#include <gloperate/base/RenderTarget.h>


namespace gloperate
{


TypedRenderTargetCapability::TypedRenderTargetCapability()
: AbstractTypedRenderTargetCapability()
{
}

TypedRenderTargetCapability::~TypedRenderTargetCapability()
{
    m_renderTargets.clear();
}

bool TypedRenderTargetCapability::hasRenderTarget(RenderTargetType type)
{
    return (m_renderTargets.count(type) > 0);
}

const RenderTarget & TypedRenderTargetCapability::renderTarget(RenderTargetType type)
{
    return m_renderTargets[type];
}

void TypedRenderTargetCapability::setRenderTarget(
    RenderTargetType type,
    globjects::ref_ptr<globjects::Framebuffer> framebuffer,
    gl::GLenum attachment,
    gl::GLenum format)
{
    m_renderTargets[type] = RenderTarget(framebuffer, attachment, format);
    setChanged(true);
}

void TypedRenderTargetCapability::resetRenderTarget(RenderTargetType type)
{
    m_renderTargets[type] = RenderTarget();
    setChanged(true);
}


} // namespace gloperate
