
#include <gloperate/base/AbstractGLContext.h>

#include <glbinding/Binding.h>

#include <globjects/globjects.h>
#include <globjects/DebugMessage.h>


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

void AbstractGLContext::initializeGLBinding()
{
    glbinding::Binding::initialize(false);
    globjects::init();
#ifdef DEBUG
    globjects::DebugMessage::enable(true);
#endif
}


} // namespace gloperate
