
#pragma once


#include <cppexpose/variant/Variant.h>

#include <gloperate/viewer/Surface.h>
#include <gloperate/viewer/input.h>
#include <gloperate/stages/base/ViewerContainer.h>


namespace gloperate
{


class Stage;
class MouseDevice;
class KeyboardDevice;
class AbstractVideoExporter;
class ImageExporter;


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
    */
    RenderSurface(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderSurface();

    /**
    *  @brief
    *    Get root pipeline
    *
    *  @return
    *    Root pipeline (never null)
    */
    Pipeline * rootPipeline() const;

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

    /**
    *  @brief
    *    Request to render this surface to a video
    *
    *  @param[in] filename
    *    Name of output video file
    *  @param[in] width
    *    Width (in pixels) of output video
    *  @param[in] height
    *    Height (in pixels) of output video
    *  @param[in] fps
    *    Frames per second of output video
    *  @param[in] length
    *    Length (in seconds) of output video
    */
    void createVideo(std::string filename, int width, int height, int fps, int seconds, std::string backend = "FFMPEGVideoEncoder");

    /**
    *  @brief
    *    Request to render this surface to an image
    *
    *  @param[in] filename
    *    Name of output image file
    *  @param[in] width
    *    Width (in pixels) of output image
    *  @param[in] height
    *    Height (in pixels) of output image
    *  @param[in] renderIterations
    *    Number of render iterations
    */
    void exportImage(std::string filename, int width, int height, int renderIterations);

    /**
    *  @brief
    *    Getter of the current video export progress in percent
    *
    *  @return
    *    Percent of the current video export progress. When nothing is being exported, it returns 0.
    */
    int exportProgress();

    /**
    *  @brief
    *    Get the available plugin names for video export backends.
    *
    *  @return
    *    Vector of the available plugin names for video export backends.
    */
    cppexpose::VariantArray videoExporterPlugins();


    // Virtual Surface functions
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


protected:
    ViewerContainer         m_viewer;         ///< Container for the rendering stage or pipeline
    unsigned long           m_frame;          ///< Frame counter
    MouseDevice           * m_mouseDevice;    ///< Device for Mouse Events
    KeyboardDevice        * m_keyboardDevice; ///< Device for Keyboard Events
    AbstractVideoExporter * m_video;          ///< Tool for rendering surface to video file
    ImageExporter         * m_imageExporter;  ///< Tool for exporting an image from surface
    bool                    m_requestVideo;   ///< Flag to request a videoTool call during next render step
    bool                    m_requestImage;   ///< Flag to request a ImageExporter call during next render step
};


} // namespace gloperate
