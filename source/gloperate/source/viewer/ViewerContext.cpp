
#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


ViewerContext::ViewerContext(AbstractGLContextFactory & contextFactory)
: m_contextFactory(contextFactory)
{
}

ViewerContext::~ViewerContext()
{
}

AbstractGLContextFactory & ViewerContext::contextFactory()
{
    return m_contextFactory;
}


} // namespace gloperate
