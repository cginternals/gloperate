
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/rendering/Camera.h>
#include <gloperate/pipeline/RenderStage.h>
#include <gloperate/pipeline/Input.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that renders a scene of animated cubes
*/
class GLOPERATE_API DemoCubeScapeStage : public RenderStage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoCubeScapeStage, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo stage that renders a scene of animated cubes"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<globjects::Texture *> texture;      ///< Texture object
    Input<float>                angle;        ///< Current angle of rotating triangle (in radians)
    Input<globjects::Texture *> colorTexture; ///< Target color texture

    // Outputs
    Output<globjects::Framebuffer *> fboOut;          ///< Pass through of target FBO
    Output<globjects::Texture *>     colorTextureOut; ///< Pass through of target color texture


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
    DemoCubeScapeStage(Environment * environment, const std::string & name = "DemoCubeScapeStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoCubeScapeStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void setupGeometry();
    void setupCamera();
    void setupProgram();


protected:
    // Rendering objects
    globjects::ref_ptr<gloperate::Camera>      m_camera;
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBuffer;
    globjects::ref_ptr<globjects::Program>     m_program;
    globjects::ref_ptr<globjects::Shader>      m_vertexShader;
    globjects::ref_ptr<globjects::Shader>      m_fragmentShader;
};


} // namespace gloperate
