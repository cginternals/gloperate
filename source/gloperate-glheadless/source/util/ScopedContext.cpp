
#include <gloperate-glheadless/util/ScopedContext.h>

#include <gloperate/base/AbstractGLContext.h>
#include <cassert>


using namespace gloperate;


namespace gloperate_glheadless {


ScopedContext::ScopedContext(AbstractGLContext * currentContext, AbstractGLContext * newContext)
: m_currentContext(currentContext)
, m_newContext(newContext)
{
    assert(currentContext);
    assert(newContext);

    m_currentContext->release();
    m_newContext->use();
}


ScopedContext::~ScopedContext()
{
    m_newContext->release();
    m_currentContext->use();
}


} // namespace gloperate_glheadless
