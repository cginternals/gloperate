
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/rendering/Camera.h>


namespace gloperate
{

class AbstractDrawable;
class Drawable;

}


/**
*  @brief
*    Demo stage that creates a simple triangle strip as a drawable
*
*  @remarks
*    This stage is part of the ShaderDemoPipeline
*/
class DemoDrawableStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoDrawableStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Demo stage that creates a simple triangle strip as a drawable"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Outputs
    Output<gloperate::AbstractDrawable *> drawable; ///< The demo drawable


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
    DemoDrawableStage(gloperate::Environment * environment, const std::string & name = "DemoDrawableStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoDrawableStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    // Rendering objects
    std::unique_ptr<gloperate::Drawable>    m_drawable;
    std::unique_ptr<globjects::VertexArray> m_vao;
    std::unique_ptr<globjects::Buffer>      m_vertexBuffer;
};
