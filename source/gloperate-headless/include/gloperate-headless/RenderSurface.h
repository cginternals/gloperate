
#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <gloperate/input/constants.h>

#include <gloperate-headless/Surface.h>


namespace gloperate
{
    class Environment;
    class Canvas;
}


namespace gloperate_headless
{


class Application;


/**
*  @brief
*    Default rendering surface for gloperate scenes
*/
class GLOPERATE_HEADLESS_API RenderSurface : public Surface
{
public:
    /**
    *  @brief
    *    Constructor
    * 
    *  @param[in] app
    *    Application instance
    *
    *  @param[in] environment
    *    Environment to which the window belongs (must NOT be null)
    */
    RenderSurface(Application * app, gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderSurface();

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
    virtual void onPaint(PaintEvent & event) override;
    virtual void onKeyPress(KeyEvent & event) override;
    virtual void onKeyRelease(KeyEvent & event) override;
    virtual void onMousePress(MouseEvent & event) override;
    virtual void onMouseRelease(MouseEvent & event) override;
    virtual void onMouseMove(MouseEvent & event) override;
    virtual void onScroll(MouseEvent & event) override;
    virtual void onIdle() override;


protected:
    gloperate::Environment           * m_environment; ///< Gloperate environment to which the window belongs (must NOT be null) 
    std::unique_ptr<gloperate::Canvas> m_canvas;      ///< Canvas that controls the rendering onto the window (must NOT be null)
    glm::ivec2                         m_deviceSize;  ///< Window size (real device pixels)
};


} // namespace gloperate_headless
