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
*    Constructor
*/
RenderTarget::RenderTarget(
    globjects::ref_ptr<globjects::Framebuffer> framebuffer
,   gl::GLenum attachment
,   gl::GLenum format)
: m_framebuffer(framebuffer)
, m_attachment(attachment)
, m_format(format)
{
}

/**
*  @brief
*    Destructor
*/
RenderTarget::~RenderTarget()
{
}

globjects::ref_ptr<globjects::Framebuffer> RenderTarget::framebuffer()
{
    return m_framebuffer;
}

gl::GLenum RenderTarget::attachment()
{
    return m_attachment;
}

gl::GLenum RenderTarget::format()
{
    return m_format;
}
} // namespace gloperate