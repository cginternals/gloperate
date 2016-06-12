
#include <gloperate/base/AbstractGLContext.h>


namespace gloperate
{


AbstractGLContext::AbstractGLContext()
{
}

AbstractGLContext::~AbstractGLContext()
{
}

glbinding::ContextHandle AbstractGLContext::handle() const
{
    return m_handle;
}

const GLContextFormat & AbstractGLContext::format() const
{
    return m_format;
}


} // namespace gloperate
