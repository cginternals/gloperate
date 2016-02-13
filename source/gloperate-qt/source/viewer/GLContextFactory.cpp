
#include <gloperate-qt/viewer/GLContextFactory.h>


namespace gloperate_qt
{


GLContextFactory::GLContextFactory()
{
}

GLContextFactory::~GLContextFactory()
{
}

gloperate::AbstractGLContext * GLContextFactory::createContext(const gloperate::GLContextFormat & format)
{
    return nullptr;
}


} // namespace gloperate_qt
