
#include <gloperate/base/AbstractGLContextFactory.h>


namespace gloperate
{


AbstractGLContextFactory::AbstractGLContextFactory()
{
}

AbstractGLContextFactory::~AbstractGLContextFactory()
{
}

gloperate::AbstractGLContext * AbstractGLContextFactory::createBestContext(const gloperate::GLContextFormat & format)
{
    // [TODO] Implement logic
    return createContext(format);
}


} // namespace gloperate
