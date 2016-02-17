
#pragma once


#include <gloperate/viewer/Surface.h>
#include <gloperate/base/ChronoTimer.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Default surface renderer for gloperate
*/
class GLOPERATE_API RenderSurface : public Surface
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    *  @param[in] renderStage
    *    Stage that renders into the surface (can be null)
    */
    RenderSurface(ViewerContext * viewerContext, Stage * stage = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderSurface();

    /**
    *  @brief
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the current context (can be null)
    */
    Stage * renderStage() const;

    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] stage
    *    Render stage that renders into the current context (can be null)
    *
    *  @remarks
    *    When setting a new render stage, the old render stage is destroyed.
    *    The surface takes ownership over the stage.
    */
    void setRenderStage(Stage * stage);

    // Virtual Surface functions
    virtual const GLContextFormat & requiredFormat() const override;
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onIdle() override;
    virtual void onResize(const glm::ivec2 & deviceSize, const glm::ivec2 & virtualSize) override;
    virtual void onRender() override;
    virtual void onKeyPress(int key) override;
    virtual void onKeyRelease(int key) override;
    virtual void onMouseMove(const glm::ivec2 & pos) override;
    virtual void onMousePress(int button, const glm::ivec2 & pos) override;
    virtual void onMouseRelease(int button, const glm::ivec2 & pos) override;
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos) override;


protected:
    Stage                  * m_renderStage; ///< Render stage that renders into the current context (can be null)
    gloperate::ChronoTimer   m_timer;       ///< Time measurement
    float                    m_time;        ///< Virtual time (in seconds)
    unsigned long            m_frame;       ///< Frame counter
};


} // namespace gloperate
