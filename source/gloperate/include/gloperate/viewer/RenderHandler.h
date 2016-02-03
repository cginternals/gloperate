
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Handler that controls the actual rendering into a context
*
*    A render handler is attached to a window or offscreen context and handles
*    the actual rendering. It should be embedded by the windowing backend
*    and receives state changes from the outside (such as window size, or mouse and keyboard
*    events) and passes them on to the rendering components.
*/
class GLOPERATE_API RenderHandler
{
public:
    /**
    *  @brief
    *    Constructor
    */
    RenderHandler();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderHandler();

    /**
    *  @brief
    *    Initialize renderer in context
    *
    *    This function is called when the renderer needs to initialize
    *    its rendering objects, i.e., whenever the OpenGL context has changed.
    *    This occurs not only when the window is created, but can also happen
    *    for example when switching between window and fullscreen mode.
    *
    *    Therefore, the renderer must be aware that initialize() and deinitialize()
    *    can be called multiple times over its lifetime and must be able to
    *    restore/re-create all necessary OpenGL objects, such as textures and geometries.
    */
    virtual void onContextInit();

    /**
    *  @brief
    *    De-Initialize renderer in context
    *
    *    This function is called when the OpenGL context of this renderer
    *    is destroyed. The renderer must release its OpenGL objects at this point.
    *
    *  @see initialize()
    */
    virtual void onContextDeinit();

    /**
    *  @brief
    *    Called when the viewer is in idle mode (no more messages to process)
    */
    virtual void onIdle();

    /**
    *  @brief
    *    Resize framebuffer
    *
    *    This function is called when the framebuffer size has been changed,
    *    e.g., due to a resize of the window.
    *
    *  @param[in] width
    *    Width (actual device pixels)
    *  @param[in] height
    *    Height (actual device pixels)
    */
    virtual void onResize(int width, int height);

    /**
    *  @brief
    *    Render scene
    *
    *    This function is called when the viewer needs to redraw its content.
    *    Use it to render the actual scene you want to display.
    */
    virtual void onRender();

    /**
    *  @brief
    *    Key pressed
    *
    *    This function is called when a key has been pressed inside the viewer.
    *
    *  @param[in] key
    *    Virtual Key Code (see gloperate::Key)
    */
    virtual void onKeyPress(int key);

    /**
    *  @brief
    *    Key released
    *
    *    This function is called when a key has been released inside the viewer.
    *
    *  @param[in] key
    *    Virtual Key Code (see gloperate::Key)
    */
    virtual void onKeyRelease(int key);

    /**
    *  @brief
    *    Mouse moved
    *
    *    This function is called when the mouse has been moved inside the viewer.
    *
    *  @param[in] x
    *    Mouse X-position (real device coordinates)
    *  @param[in] y
    *    Mouse Y-position (real device coordinates)
    */
    virtual void onMouseMove(int x, int y);

    /**
    *  @brief
    *    Mouse button pressed
    *
    *    This function is called when a mouse button has been pressed inside the viewer.
    *
    *  @param[in] button
    *    Mouse button (see gloperate::MouseButton)
    *  @param[in] x
    *    Mouse X-position (real device coordinates)
    *  @param[in] y
    *    Mouse Y-position (real device coordinates)
    */
    virtual void onMousePress(int button, int x, int y);

    /**
    *  @brief
    *    Mouse button released
    *
    *    This function is called when a mouse button has been released inside the viewer.
    *
    *  @param[in] button
    *    Mouse button (see gloperate::MouseButton)
    *  @param[in] x
    *    Mouse X-position (real device coordinates)
    *  @param[in] y
    *    Mouse Y-position (real device coordinates)
    */
    virtual void onMouseRelease(int button, int x, int y);

    /**
    *  @brief
    *    Timer event
    *
    *    This function is called when a timer has fired.
    *
    *  @param[in] id
    *    Timer ID
    */
    virtual void onTimer(int id);
};


} // namespace gloperate
