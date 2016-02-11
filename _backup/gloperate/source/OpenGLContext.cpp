
#include <gloperate/base/OpenGLContext.h>

#include <cassert>
#include <map>

#include <glbinding/ContextInfo.h>
#include <glbinding/ProcAddress.h>
#include <glbinding/Version.h>
#include <glbinding/gl/gl.h>

#include <globjects/base/baselogging.h>


using namespace gl;
using namespace globjects;


namespace gloperate
{


glbinding::Version OpenGLContext::retrieveVersion()
{
    assert(0 != glbinding::getCurrentContext());

    GLint major = -1;
    GLint minor = -1;

    glGetIntegerv(GLenum::GL_MAJOR_VERSION, &major); // major version
    glGetIntegerv(GLenum::GL_MINOR_VERSION, &minor); // minor version

    if (major < 0 && minor < 0) // probably a context < 3.0 with no support for GL_MAJOR/MINOR_VERSION
    {
        const GLubyte * vstr = glGetString(GLenum::GL_VERSION);
        if (!vstr)
            return glbinding::Version();

        assert(vstr[1] == '.');

        assert(vstr[0] >= '0'  && vstr[0] <= '9');
        major = vstr[0] - '0';

        assert(vstr[2] >= '0'  && vstr[2] <= '9');
        minor = vstr[2] - '0';
    }
    return glbinding::Version(major, minor);
}

const std::string & OpenGLContext::swapIntervalString(const SwapInterval interval)
{
    static const std::map<SwapInterval, std::string> swapiIdentifier = {
        { SwapInterval::NoVerticalSyncronization,       "NoVerticalSyncronization" }
    ,   { SwapInterval::VerticalSyncronization,         "VerticalSyncronization" }
    ,   { SwapInterval::AdaptiveVerticalSyncronization, "AdaptiveVerticalSyncronization" } };

    return swapiIdentifier.at(interval);
}

std::string OpenGLContext::version()
{
    assert(0 != glbinding::getCurrentContext());
    return glbinding::ContextInfo::version().toString();
}

std::string OpenGLContext::vendor()
{
    assert(0 != glbinding::getCurrentContext());
    return glbinding::ContextInfo::vendor();
}

std::string OpenGLContext::renderer()
{
    assert(0 != glbinding::getCurrentContext());
    return glbinding::ContextInfo::renderer();
}


OpenGLContext::OpenGLContext()
: m_swapInterval(SwapInterval::NoVerticalSyncronization)
{
}

OpenGLContext::~OpenGLContext()
{
}

bool OpenGLContext::isValid() const
{
    return 0 < handle();
}

OpenGLContext::SwapInterval OpenGLContext::swapInterval() const
{
    return m_swapInterval;
}

bool OpenGLContext::setSwapInterval(const SwapInterval interval)
{
    if (interval == m_swapInterval)
        return true;

    bool result(false);

    if (handle() != glbinding::getCurrentContext())
    {
        warning() << "Setting swap interval skipped (another context was already current).";
        return false;
    }

    assert(0 != glbinding::getCurrentContext());

#ifdef WIN32

    using SWAPINTERVALEXTPROC = bool(*)(int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT(nullptr);

    if (!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(glbinding::getProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(static_cast<int>(interval));

#elif __APPLE__

    warning("ToDo: Setting swap interval is currently not implemented for __APPLE__");

#else

    using SWAPINTERVALEXTPROC = int(*)(int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

    if(!glXSwapIntervalSGI)
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(glbinding::getProcAddress("glXSwapIntervalSGI"));
    if(glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(static_cast<int>(interval));

#endif

    if (result)
        m_swapInterval = interval;

    if(!result)
        warning("Setting swap interval to % failed.", swapIntervalString(interval));

    return result;
}

glbinding::ContextHandle OpenGLContext::tryFetchHandle()
{
    const glbinding::ContextHandle handle = glbinding::getCurrentContext();

    if (0 == handle)
        critical("Acquiring OpenGL context handle failed.");

    return handle;
}


} // namespace gloperate
