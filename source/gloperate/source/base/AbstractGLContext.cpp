
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

const GLContextFormat & AbstractGLContext::format() const
{
    return m_format;
}

void AbstractGLContext::initializeBindings(glbinding::GetProcAddress functionPointerResolver)
{
    // Initialize globjects and glbinding
    globjects::init(functionPointerResolver);

    // Enable debug messages
#ifndef NDEBUG
    globjects::DebugMessage::enable(true);
#endif
}


} // namespace gloperate
