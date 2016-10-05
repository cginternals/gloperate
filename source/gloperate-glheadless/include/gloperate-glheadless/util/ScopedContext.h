
#pragma once


#include <gloperate-glheadless/gloperate-glheadless_api.h>


namespace gloperate {
class AbstractGLContext;
}


namespace gloperate_glheadless {


/**
*  @brief
*    RAII helper to temporarily change the current context
*
*  Activates the given new context on construction and restores the previous
*  context upon destruction.
*/
class GLOPERATE_GLHEADLESS_API ScopedContext
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] currentContext
    *    The currently active context, to be restored on destruction (must not be null)
    *
    *  @param[in] newContext
    *    The new context that should temporarily be made current (must not be null)
    */
    ScopedContext(gloperate::AbstractGLContext * currentContext, gloperate::AbstractGLContext * newContext);
    ~ScopedContext();

    ScopedContext(const ScopedContext&) = delete;
    ScopedContext(ScopedContext&&) = delete;

    ScopedContext& operator=(const ScopedContext&) = delete;
    ScopedContext& operator=(ScopedContext&&) = delete;


private:
    gloperate::AbstractGLContext * m_currentContext; ///< The current context at the time of construction
    gloperate::AbstractGLContext * m_newContext;     ///< The new context to be made current
};


} // namespace gloperate_glheadless
