
#pragma once

#include <memory>
#include <queue>
#include <set>
#include <string>

#include <glm/vec2.hpp>

#include <eglbinding/egl/types.h>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-headless/gloperate-headless_api.h>


namespace gloperate
{
    class AbstractGLContext;
}


namespace gloperate_headless
{


class GLContext;


/**
*  @brief
*    Surface with OpenGL context
*/
class GLOPERATE_HEADLESS_API Surface
{
    friend class Application;


public:
    /**
    *  @brief
    *    Get list of surfaces
    *
    *  @return
    *    List of open surfaces
    */
    static const std::set<Surface*> & instances();


private:
    static std::set<Surface*> s_instances;   ///< List of window instances


public:
    /**
    *  @brief
    *    Constructor
    */
    Surface(egl::EGLDisplay display);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Surface();

    /**
    *  @brief
    *    Set OpenGL context format
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *
    *  @return
    *    'true' if the format could be set, else 'false'
    *
    *  @remarks
    *    The context format can only be set before the window
    *    has been created. Afterwards, the function will fail
    *    and the context format will not be changed.
    */
    bool setContextFormat(const gloperate::GLContextFormat & format);

    /**
    *  @brief
    *    Create window
    *
    *  @return
    *    'true' if window could be created, else 'false'
    *
    *  @remarks
    *    If the window has already been created, this function will fail.
    */
    bool create();

    /**
    *  @brief
    *    Destroy window
    */
    void destroy();

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context (can be nullptr)
    */
    const GLContext * context() const;

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context (can be nullptr)
    */
    GLContext * context();

    /**
    *  @brief
    *    Check if application quits when windows gets closed
    *
    *  @return
    *    'true' if window quits the application when closed, else 'false'
    */
    bool quitsOnDestroy() const;

    /**
    *  @brief
    *    Set if application quits when the window gets closed
    *
    *  @param[in] quitOnDestroy
    *    'true' if window quits the application when closed, else 'false'
    *
    *  @remarks
    *    If enabled, this causes an application wide quit message to be posted
    *    when the window gets destroyed. Hence, the MainLoop will be quit
    *    and all other remaining windows destroyed.
    */
    void setQuitOnDestroy(bool quitOnDestroy);

    /**
    *  @brief
    *    Schedule a repaint on the window
    *
    *  @remarks
    *    When calling this function, a redraw-event will be sent to the
    *    message queue, causing the window to be repainted within the
    *    next event processing.
    */
    void repaint();

    /**
    *  @brief
    *    Swap front and back buffer
    */
    void swap();

    /**
    *  @brief
    *    Called once every mainloop iteration
    */
    void idle();

    /**
    *  @brief
    *    Get surface title
    *
    *  @return
    *    Surface title
    */
    const std::string & title() const;

    /**
    *  @brief
    *    Set surface title
    *
    *  @param[in] title
    *    Surface title
    */
    void setTitle(const std::string & title);


protected:
    /**
    *  @brief
    *    Get EGL surface handle
    *
    *  @return
    *    EGL surface, can be nullptr
    */
    egl::EGLSurface internalSurface() const;

    /**
    *  @brief
    *    Check if window has events waiting
    *
    *  @return
    *    'true' if events are waiting in the window's event queue, else 'false'
    */
    bool hasPendingEvents();

    /**
    *  @brief
    *    Process all waiting events and dispatch them to event handling
    */
    void processEvents();

    /**
    *  @brief
    *    Check for repaint event
    *
    *  @remarks
    *    If repaint() has been called on the window, a repaint event
    *    will be added to the window's event queue. This needs to be
    *    done as a separate call to avoid event processing to get stuck
    *    in an endless loop. Make sure to call Application::pollEvents()
    *    and updateRepaintEvent() in turn.
    */
    void updateRepaintEvent();

    /**
    *  @brief
    *    Recreates GLFW window using current settings
    *
    *  @return
    *    'true' on success, else 'false'
    */
    bool recreateSurface();

    /**
    *  @brief
    *    Create internal surface and OpenGL context
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *  @param[in] width
    *    Window width (in pixels)
    *  @param[in] height
    *    Window height (in pixels)
    *  @param[in] display
    *    EGL display handle, can be nullptr
    *
    *  @return
    *    'true' if context could be created, else 'false'
    *
    *  @remarks
    *    This function will actually create a new window with the given context
    *    format, so any previously obtained window IDs will be rendered invalid.
    */
    bool createInternalSurface(const gloperate::GLContextFormat & format, int width, int height, bool setContextActive, egl::EGLDisplay display);

    /**
    *  @brief
    *    Destroy internal window and OpenGL context
    *
    *  @remarks
    *    This function will actually destroy the current window.
    */
    void destroyInternalSurface();

    // Event handlers, to be overwritten in derived classes
    virtual void onContextInit();
    virtual void onContextDeinit();
    virtual void onIdle();


protected:
    std::string                              m_title;         ///< Window title
    gloperate::GLContextFormat               m_format;        ///< The desired OpenGL context format

    egl::EGLDisplay                          m_display;       ///< EGL display (can be nullptr)
    glm::ivec2                               m_size;          ///< Size of window when returned from fullscreen mode
    bool                                     m_quitOnDestroy; ///< Quit application when window is closed?
    bool                                     m_needsRepaint;  ///< Has a repaint be scheduled?
    std::unique_ptr<GLContext>               m_context;       ///< OpenGL context & EGL Surface (can be nullptr)
};


} // namespace gloperate_headless
