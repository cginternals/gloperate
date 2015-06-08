/******************************************************************************\
* gloperate
*
* Copyright (C) 2014 Computer Graphics Systems Group at the
* Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/

#include <gloperate/base/RenderTarget.h>

namespace gloperate
{
/**
*  @brief
*    Standard Constructor
*/
RenderTarget::RenderTarget()
    :   m_attachment()
    ,   m_format()
    ,   m_valid(false)
{
}

/**
*  @brief
*    Constructor
*/
RenderTarget::RenderTarget(
    globjects::ref_ptr<globjects::Framebuffer> framebuffer,
    gl::GLenum attachment,   
    gl::GLenum format)
    :   m_framebuffer(framebuffer)
    ,   m_attachment(attachment)
    ,   m_format(format)
    ,   m_valid(true)
{
}

/**
*  @brief
*    Copy Constructor
*/
RenderTarget::RenderTarget(
    const RenderTarget & renderTarget)
    :   m_framebuffer(renderTarget.m_framebuffer)
    ,   m_attachment(renderTarget.m_attachment)
    ,   m_format(renderTarget.m_format)
    ,   m_valid(renderTarget.m_valid)
{
}


/**
*  @brief
*    Destructor
*/
RenderTarget::~RenderTarget()
{
}

bool RenderTarget::isValid() const
{
    return m_valid;
}

globjects::ref_ptr<globjects::Framebuffer> RenderTarget::framebuffer() const
{
    return m_framebuffer;
}

gl::GLenum RenderTarget::attachment() const
{
    return m_attachment;
}

gl::GLenum RenderTarget::format() const
{
    return m_format;
}
} // namespace gloperate
