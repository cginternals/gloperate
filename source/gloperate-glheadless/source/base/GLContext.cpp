
#include "gloperate-glheadless/base/GLContext.h"

#include <cassert>

#include <glheadless/Context.h>
#include <glheadless/ContextFactory.h>

#include <gloperate/base/GLContextUtils.h>


using namespace gloperate;


namespace gloperate_headless
{


GLContext * GLContext::current()
{
    auto glheadlessContext = glheadless::ContextFactory::getCurrent();
    if (glheadlessContext->valid())
    {
        // GLContext constructor calls makeCurrent() + doneCurrent() on the given context,
        // but since we pass the current context, we need to release it before and make
        // it current again afterwards
        glheadlessContext->doneCurrent();
        auto context = new GLContext(std::move(glheadlessContext));
        context->use();
        return context;
    }
    return nullptr;
}


GLContext::GLContext(std::unique_ptr<glheadless::Context> && context)
: m_glhContext(std::move(context))
{
    assert(m_glhContext);

    // Activate context
    m_glhContext->makeCurrent();

    // Read context handle
    m_handle = GLContextUtils::tryFetchHandle();

    // Read context format
    m_format = GLContextUtils::retrieveFormat();

    // Deactivate context
    m_glhContext->doneCurrent();
}


GLContext::~GLContext()
{
}


glheadless::Context * GLContext::glheadlessContext() const
{
    return m_glhContext.get();
}


void GLContext::use() const
{
    m_glhContext->makeCurrent();
}


void GLContext::release() const
{
    m_glhContext->doneCurrent();
}


} // namespace gloperate_headless
