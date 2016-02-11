
#include <gloperate-qt/viewer/Context.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>

#include <gloperate/base/OpenGLContextUtils.h>
#include <gloperate/base/ContextFormat.h>

#include <gloperate-qt/viewer/OpenGLWindow.h>


using namespace gl;
using namespace globjects;
using namespace gloperate;


namespace gloperate_qt
{


Context::Context(OpenGLWindow * window, QOpenGLContext * context)
: m_window(window)
, m_context(context)
, m_format(nullptr)
{
    assert(window);
    assert(context);

    Context::makeCurrent(m_context, m_window);

    glbinding::Binding::initialize(false);

    m_handle = OpenGLContextUtils::tryFetchHandle();

    Context::doneCurrent(m_context);
}

Context::~Context()
{
    delete m_format;
}

glbinding::ContextHandle Context::handle() const
{
    return m_handle;
}

const ContextFormat & Context::format() const
{
    assert(isValid());

    if (m_format)
    {
        return *m_format;
    }

    // Create and retrieve format if not done already
    m_format = new ContextFormat();

    Context::makeCurrent(m_context, m_window);

    m_format->setVersion(OpenGLContextUtils::retrieveVersion());

    GLint i;
    GLboolean b;

    i = -1; glGetIntegerv(GLenum::GL_DEPTH_BITS, &i);
    m_format->setDepthBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_STENCIL_BITS, &i);
    m_format->setStencilBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_RED_BITS, &i);

    m_format->setRedBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_GREEN_BITS, &i);
    m_format->setGreenBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_BLUE_BITS, &i);
    m_format->setBlueBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_ALPHA_BITS, &i);
    m_format->setAlphaBufferSize(i);
    i = -1; glGetIntegerv(GLenum::GL_SAMPLES, &i);
    m_format->setSamples(i);
    b = GL_FALSE; glGetBooleanv(GLenum::GL_STEREO, &b);
    m_format->setStereo(b == GL_TRUE);

    Context::doneCurrent(m_context);

    return *m_format;
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
