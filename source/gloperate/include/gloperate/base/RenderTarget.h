/******************************************************************************\
* gloperate
*
* Copyright (C) 2014 Computer Graphics Systems Group at the
* Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <globjects/Framebuffer.h>
#include <globjects/base/ref_ptr.h>
#include <gloperate/gloperate_api.h>

namespace gloperate
{

class GLOPERATE_API RenderTarget
{
public:
    RenderTarget();

    RenderTarget(
        globjects::ref_ptr<globjects::Framebuffer> framebuffer,
        gl::GLenum attachment,
        gl::GLenum format);

    RenderTarget(
        const RenderTarget & renderTarget);

    virtual ~RenderTarget();

    bool isValid() const;

    globjects::ref_ptr<globjects::Framebuffer> framebuffer() const;
    gl::GLenum attachment() const;
    gl::GLenum format() const;

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_framebuffer;
    gl::GLenum m_attachment;
    gl::GLenum m_format;
    bool m_valid;
};

} // namespace gloperate