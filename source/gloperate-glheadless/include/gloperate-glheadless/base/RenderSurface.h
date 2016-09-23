
#pragma once


#include <gloperate-glheadless/base/OffscreenSurface.h>

#include <globjects/base/ref_ptr.h>


namespace globjects {
    class Framebuffer;
}


namespace gloperate{
    class Environment;
    class Canvas;
    class Stage;
}


namespace gloperate_headless
{


/**
*  @brief
*    Window that renders a gloperate scene
*/
class GLOPERATE_HEADLESS_API RenderSurface : public OffscreenSurface {
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the window belongs (must NOT be null!)
    */
    explicit RenderSurface(gloperate::Environment * environment);

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
    *    Gloperate environment (cannot be null)
    */
    gloperate::Environment * environment() const;

    /**
    *  @brief
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the window (can be null)
    */
    gloperate::Stage * renderStage() const;

    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] stage
    *    Render stage that renders into the window (can be null)
    *
    *  @remarks
    *    When setting a new render stage, the old render stage is destroyed.
    *    The window takes ownership over the stage.
    */
    void setRenderStage(gloperate::Stage * stage);


protected:
    // Virtual OpenGLWindow functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onResize(int width, int height) override;
    virtual void onPaint() override;


protected:
    gloperate::Environment                     * m_environment; ///< Gloperate environment to which the window belongs (must NOT be null)
    std::unique_ptr<gloperate::Canvas>           m_canvas;      ///< Canvas that renders onto the window (must NOT be null)
    globjects::ref_ptr<globjects::Framebuffer>   m_defaultFBO;  ///< Default framebuffer for rendering
};


} // namespace gloperate_headless
