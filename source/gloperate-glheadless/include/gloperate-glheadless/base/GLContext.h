
#pragma once


#include <memory>

#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-glheadless/gloperate-glheadless_api.h>


namespace glheadless
{
class Context;
} // namespace glheadless


namespace gloperate_headless
{


/**
*  @brief
*    OpenGL context implementation based on glheadless
*
*  The context does not have a default framebuffer, i.e., all rendering
*  must be done through a custom frame buffer object.
*/
class GLOPERATE_HEADLESS_API GLContext : public gloperate::AbstractGLContext
{
    friend class GLContextFactory;


public:
    /**
    *  @brief
    *    Get the current context, if any
    *
    *  @return
    *    glheadless OpenGL context (can be null)
    *
    *  @remarks
    *    The result will use a glheadless representation of the current context,
    *    regardless of its origin (Qt, GLFW, etc.). This representation will be
    *    "non-owning", i.e., destroying it will not destroy the context itself.
    *    Use this to restore the current context after activating another.
    */
    static GLContext * current();


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] context
    *    glheadless OpenGL context
    */
    explicit GLContext(std::unique_ptr<glheadless::Context> && glhContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContext();

    /**
    *  @brief
    *    Get glheadless OpenGL context
    *
    *  @return
    *    glheadless OpenGL context
    */
    glheadless::Context * glheadlessContext() const;

    // Virtual gloperate::AbstractGLContext functions
    virtual void use() const override;
    virtual void release() const override;


protected:
    std::unique_ptr<glheadless::Context> m_glhContext; ///< Internal glheadless context representation
};


} // namespace gloperate_headless
