
#include <gloperate-headless/getProcAddress.h>

#include <EGL/egl.h>


namespace gloperate_headless
{

ProcAddress getProcAddress(const char * name)
{
    return eglGetProcAddress(name);
}


} // namespace gloperate_headless
