
#pragma once


#include <gloperate/base/AbstractGLContextFactory.h>

#include <memory>

#include <gloperate-glheadless/gloperate-glheadless_api.h>


namespace glheadless
{
struct ContextFormat;
class Context;
} // namespace glheadless


namespace gloperate_glheadless
{


/**
*  @brief
*    OpenGL context factory
*
*  @remarks
*    Supports creation of shared context. Use setSharedContext() or setShareCurrent() to configure
*    (setSharedContext takes precedence). Contexts created through this factory will shared their
*    state with the configured one.
*/
class GLOPERATE_GLHEADLESS_API GLContextFactory : public gloperate::AbstractGLContextFactory
{
public:
    /**
    *  @brief
    *    Convert context format description into glheadless version
    *
    *  @param[in] format
    *    OpenGL context format description (gloperate version)
    *
    *  @return
    *    OpenGL context format description (glheadless version)
    *
    *  @remarks
    *    As glheadless contexts do not have a default framebuffer,
    *    swap behavior and buffer sizes are ignored.
    */
    static glheadless::ContextFormat toGlhContextFormat(const gloperate::GLContextFormat & format);


public:
    /**
    *  @brief
    *    Constructor
    */
    GLContextFactory();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    /**
    *  @brief
    *    Get the shared context
    *
    *  @return
    *    OpenGL context (can be null)
    */
    gloperate::AbstractContext * sharedContext() const;

    /**
    *  @brief
    *    Set the shared context
    *
    *  @param[in] sharedContext
    *    OpenGL context (can be null)
    *
    *  @remarks
    *    If non-null, contexts created though this factory share their state
    *    with this context. The shared context can originate from any backend.
    */
    void setSharedContext(gloperate::AbstractContext * sharedContext);

    /**
    *  @brief
    *    Get share current status
    *
    *  @return
    *    'true', if share current is on
    */
    bool shareCurrent() const;

    /**
    *  @brief
    *    Set share current status
    *
    *  @param[in] shareCurrent
    *    'true', if share current is on
    *
    *  @remarks
    *    If on, contexts created through this factory share their state with
    *    context that is active at the time of creation. This context can
    *    originate from any backend.
    */
    void setShareCurrent(bool shareCurrent);

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual gloperate::AbstractGLContext * createContext(const gloperate::GLContextFormat & format) override;


private:
    std::unique_ptr<glheadless::Context> createGlhContext(glheadless::Context * sharedGlhContext, const gloperate::GLContextFormat & format);


private:
    gloperate::AbstractContext * m_sharedContext; ///< The context to share state with
    bool                         m_shareCurrent;  ///< Flag indicating whether to share state with the active context
};


} // namespace gloperate_glheadless
