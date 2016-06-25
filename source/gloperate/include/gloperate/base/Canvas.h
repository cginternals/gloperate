
#pragma once


#include <gloperate/base/AbstractCanvas.h>
#include <gloperate/base/PipelineContainer.h>
#include <gloperate/input/constants.h>


namespace gloperate
{


class Stage;
class MouseDevice;
class KeyboardDevice;
class AbstractVideoExporter;
class ImageExporter;


/**
*  @brief
*    Default canvas renderer for gloperate
*/
class GLOPERATE_API Canvas : public AbstractCanvas
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the canvas belongs (must NOT be null!)
    */
    Canvas(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Canvas();

    /**
    *  @brief
    *    Get pipeline container
    *
    *  @return
    *    Pipeline container (never null)
    */
    PipelineContainer * pipelineContainer() const;

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
    *    The canvas takes ownership over the stage.
    */
    void setRenderStage(Stage * stage);

    // Virtual AbstractCanvas functions
    virtual glm::vec4 deviceViewport() override;
    virtual glm::vec4 virtualViewport() override;
    virtual void onUpdate() override;
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport) override;
    virtual void onBackgroundColor(float red, float green, float blue) override;
    virtual void onRender(globjects::Framebuffer * targetFBO = nullptr) override;
    virtual void onKeyPress(int key, int modifier) override;
    virtual void onKeyRelease(int key, int modifier) override;
    virtual void onMouseMove(const glm::ivec2 & pos) override;
    virtual void onMousePress(int button, const glm::ivec2 & pos) override;
    virtual void onMouseRelease(int button, const glm::ivec2 & pos) override;
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos) override;
    virtual void createVideo(std::string filename, int width, int height, int fps, int seconds, std::string backend = "FFMPEGVideoExporter") override;
    virtual void exportImage(std::string filename, int width, int height, int renderIterations) override;
    virtual int exportProgress() override;
    virtual cppexpose::VariantArray videoExporterPlugins() override;


protected:
    PipelineContainer       m_pipelineContainer; ///< Container for the rendering stage or pipeline
    unsigned long           m_frame;             ///< Frame counter
    MouseDevice           * m_mouseDevice;       ///< Device for Mouse Events
    KeyboardDevice        * m_keyboardDevice;    ///< Device for Keyboard Events
    AbstractVideoExporter * m_videoExporter;     ///< Tool for rendering canvas to video file
    ImageExporter         * m_imageExporter;     ///< Tool for exporting canvas to image file
    bool                    m_requestVideo;      ///< Flag to request a videoTool call during next render step
    bool                    m_requestImage;      ///< Flag to request a ImageExporter call during next render step
};


} // namespace gloperate
