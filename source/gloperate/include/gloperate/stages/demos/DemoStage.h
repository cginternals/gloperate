
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/viewer/Timer.h>
#include <gloperate/rendering/Camera.h>
#include <gloperate/pipeline/RenderStage.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that renders a simple triangle onto the screen
*/
class GLOPERATE_API DemoStage : public RenderStage
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    DemoStage(ViewerContext * viewerContext, const std::string & name = "DemoStage", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoStage();


protected:
    /**
    *  @brief
    *    Mark output as invalid (causing viewer to repaint the stage)
    */
    void invalidateOutput();

    // Virtual Stage functions
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void createAndSetupCamera();
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    // Rendering objects
    globjects::ref_ptr<gloperate::Camera>      m_camera;
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer>      m_buffer;
    globjects::ref_ptr<globjects::Texture>     m_texture;
    globjects::ref_ptr<globjects::Program>     m_program;
    globjects::ref_ptr<globjects::Shader>      m_vertexShader;
    globjects::ref_ptr<globjects::Shader>      m_fragmentShader;

    // Tools
    Timer m_timer;

    // Status
    float m_time;   ///< Virtual time (in seconds)
    float m_angle;  ///< Current angle of rotating triangle (in radians)
};


} // namespace gloperate
