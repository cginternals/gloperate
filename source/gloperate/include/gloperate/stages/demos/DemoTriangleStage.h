
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/rendering/Camera.h>
#include <gloperate/pipeline/RenderStage.h>
#include <gloperate/pipeline/Input.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that renders a rotating triangle onto the screen
*
*  @remarks
*    This stage is part of the DemoPipeline
*/
class GLOPERATE_API DemoTriangleStage : public RenderStage
{
    CPPEXPOSE_DECLARE_COMPONENT(DemoTriangleStage, gloperate::Stage)


public:
    // Inputs
    Input<globjects::Texture *> texture;  ///< Texture object
    Input<float>                angle;    ///< Current angle of rotating triangle (in radians)


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
    DemoTriangleStage(ViewerContext * viewerContext, const std::string & name = "DemoTriangleStage", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoTriangleStage();


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
