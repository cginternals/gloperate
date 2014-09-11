
#include <gloperate/AbstractContext.h>

#include <cassert>
#include <map>

#include <globjects/base/baselogging.h>


#ifdef WIN32
    #include <windows.h>
    #include <gl/GL.h>
    #include <gl/GLU.h>
#else
    #include <GL/glx.h>
    #include <GL/GLUx.h>
#endif


using namespace globjects;

namespace gloperate
{

const std::string & AbstractContext::swapIntervalString(const SwapInterval interval)
{
    static const std::map<SwapInterval, std::string> swapiIdentifier = {
        { SwapInterval::NoVerticalSyncronization,       "NoVerticalSyncronization" }
    ,   { SwapInterval::VerticalSyncronization,         "VerticalSyncronization" }
    ,   { SwapInterval::AdaptiveVerticalSyncronization, "AdaptiveVerticalSyncronization" } };

    return swapiIdentifier.at(interval);
}


AbstractContext::AbstractContext()
: m_swapInterval(SwapInterval::NoVerticalSyncronization)
{
}

AbstractContext::~AbstractContext()
{
}

bool AbstractContext::isValid() const
{
	return 0 < handle();
}

const ContextFormat & AbstractContext::format() const
{
	return m_format;
}

glbinding::ContextHandle AbstractContext::tryFetchHandle()
{
	assert(isValid());
    if (!isValid())
		return 0;

    makeCurrent();

    const glbinding::ContextHandle handle = glbinding::getCurrentContext();

    if (0 == handle)
        critical("Acquiring OpenGL context handle failed.");

	doneCurrent();

    return handle;
}

AbstractContext::SwapInterval AbstractContext::swapInterval() const
{
    return m_swapInterval;
}

bool AbstractContext::setSwapInterval(const SwapInterval interval)
{
    if(interval == m_swapInterval)
        return true;

    bool result(false);

#ifdef WIN32

    typedef bool (WINAPI * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

    if(!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
    if(wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(static_cast<int>(interval));

#elif __APPLE__

    warning("ToDo: Setting swap interval is currently not implemented for __APPLE__");

#else

    typedef int (APIENTRY * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

    if(!glXSwapIntervalSGI)
    {
        const GLubyte * sgi = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(glXGetProcAddress(sgi));
    }
    if(glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(static_cast<int>(interval));

#endif

    if(!result)
        warning("Setting swap interval to % failed.", swapIntervalString(interval));
    else
        m_swapInterval = interval;

    return result;
}

bool AbstractContext::verticalSync() const
{
    return m_swapInterval != SwapInterval::NoVerticalSyncronization;
}

bool AbstractContext::setVerticalSync(const bool enable)
{
    return setSwapInterval(static_cast<SwapInterval>(enable ? 1 : 0));
}

} // namespace gloperate
