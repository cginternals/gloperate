
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
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


class Drawable;


/**
*  @brief
*    Demo stage that creates a simple triangle strip as a drawable
*
*  @remarks
*    This stage is part of the ShaderDemoPipeline
*/
class GLOPERATE_API DemoDrawableStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoDrawableStage, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo stage that creates a simple triangle strip as a drawable"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Outputs
    Output<gloperate::Drawable *> drawable; ///< The demo drawable


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
    DemoDrawableStage(Environment * environment, const std::string & name = "DemoDrawableStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoDrawableStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;


protected:
    // Rendering objects
    globjects::ref_ptr<Drawable>               m_drawable;
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBuffer;
};


} // namespace gloperate
