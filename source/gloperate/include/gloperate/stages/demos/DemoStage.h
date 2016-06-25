
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Timer.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/rendering/Camera.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that renders a simple triangle onto the screen
*/
class GLOPERATE_API DemoStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoStage, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo stage that renders a simple triangle onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface<Stage> renderInterface; ///< Interface for rendering into a viewer


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    DemoStage(Environment * environment, const std::string & name = "DemoStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoStage();


protected:
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
    gloperate::Camera                          m_camera;
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
