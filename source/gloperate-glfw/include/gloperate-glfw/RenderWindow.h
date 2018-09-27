
#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <gloperate/input/constants.h>

#include <gloperate-glfw/Window.h>


namespace gloperate
{
    class Environment;
    class Canvas;
}


namespace gloperate_glfw
{


/**
*  @brief
*    Default rendering window for gloperate scenes
*/
class GLOPERATE_GLFW_API RenderWindow : public Window
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the window belongs (must NOT be null)
    */
    RenderWindow(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderWindow();

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment (cannot be null)
    */
    const gloperate::Environment * environment() const;

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment (cannot be null)
    */
    gloperate::Environment * environment();

    /**
    *  @brief
    *    Get canvas
    *
    *  @return
    *    Canvas that is rendered on
    */
    gloperate::Canvas * canvas() const;


protected:
    // Virtual Window functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onResize(ResizeEvent & event) override;
    virtual void onFramebufferResize(ResizeEvent & event) override;
    virtual void onMove(MoveEvent & event) override;
    virtual void onPaint(PaintEvent & event) override;
    virtual void onKeyPress(KeyEvent & event) override;
    virtual void onKeyRelease(KeyEvent & event) override;
    virtual void onMousePress(MouseEvent & event) override;
    virtual void onMouseRelease(MouseEvent & event) override;
    virtual void onMouseMove(MouseEvent & event) override;
    virtual void onMouseEnter(MouseEnterEvent & event) override;
    virtual void onMouseLeave(MouseLeaveEvent & event) override;
    virtual void onScroll(MouseEvent & event) override;
    virtual void onFocus(FocusEvent & event) override;
    virtual void onIconify(IconifyEvent & event) override;
    virtual void onIdle() override;

    /**
    *  @brief
    *    Convert GLFW mouse button into gloperate mouse button
    *
    *  @param[in] button
    *    GLFW mouse button
    *
    *  @return
    *    gloperate mouse button
    */
    gloperate::MouseButton fromGLFWMouseButton(int button) const;

    /**
    *  @brief
    *    Convert GLFW key code into gloperate key code
    *
    *  @param[in] key
    *    GLFW key code
    *
    *  @return
    *    gloperate key code
    */
    gloperate::Key fromGLFWKeyCode(int key) const;

    /**
    *  @brief
    *    Convert GLFW modifier code into gloperate modifier code
    *
    *  @param[in] key
    *    GLFW key modifier
    *
    *  @return
    *    gloperate key modifier
    */
    gloperate::KeyModifier fromGLFWModifier(int modifier) const;


protected:
    gloperate::Environment           * m_environment; ///< Gloperate environment to which the window belongs (must NOT be null) 
    std::unique_ptr<gloperate::Canvas> m_canvas;      ///< Canvas that controls the rendering onto the window (must NOT be null)
    glm::ivec2                         m_deviceSize;  ///< Window size (real device pixels)
    glm::ivec2                         m_virtualSize; ///< Window size (virtual pixel size)
};


} // namespace gloperate_glfw
