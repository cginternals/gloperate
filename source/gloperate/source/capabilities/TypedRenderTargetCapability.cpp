/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/TypedRenderTargetCapability.h>


namespace gloperate {


/**
*  @brief
*    Constructor
*/
TypedRenderTargetCapability::TypedRenderTargetCapability()
:AbstractTypedRenderTargetCapability()
{
}

/**
*  @brief
*    Destructor
*/
TypedRenderTargetCapability::~TypedRenderTargetCapability()
{
    m_renderTargets.clear();
}

const RenderTarget & TypedRenderTargetCapability::renderTarget(RenderTargetType type)
{
    return *(m_renderTargets.at(type));
}

bool TypedRenderTargetCapability::hasRenderTarget(RenderTargetType type)
{
    return (m_renderTargets.count(type) > 0);
}

void TypedRenderTargetCapability::setRenderTarget(
    RenderTargetType type,
    globjects::ref_ptr<globjects::Framebuffer> framebuffer,
    gl::GLenum attachment,
    gl::GLenum format)
{
    if (hasRenderTarget(type))
    {
        delete m_renderTargets.at(type);
    }

    m_renderTargets[type] = new RenderTarget(framebuffer, attachment, format);
}

} // namespace gloperate
