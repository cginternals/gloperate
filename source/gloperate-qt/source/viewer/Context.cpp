
#include <gloperate-qt/viewer/Context.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>

#include <gloperate-qt/viewer/OpenGLWindow.h>


using namespace gl;
using namespace globjects;
using namespace gloperate;


namespace gloperate_qt
{


Context::Context(OpenGLWindow * window, QOpenGLContext * context)
: m_window(window)
, m_context(context)
{
    assert(window);
    assert(context);

    // Activate context
    Context::makeCurrent(m_context, m_window);

    // Initialize glbinding in that context
    glbinding::Binding::initialize(false);

    // Read context handle
    m_handle = GLContextUtils::tryFetchHandle();

    // Read context format
    assert(isValid());
    m_format = GLContextUtils::retrieveFormat();

    // Deactivate context
    Context::doneCurrent(m_context);
}

Context::~Context()
{
}

void Context::use() const
{
    if (m_window && m_context)
    {
        Context::makeCurrent(m_context, m_window);
    }
}

void Context::release() const
{
    if (m_context)
    {
        Context::doneCurrent(m_context);
    }
}


} // namespace gloperate_qt
