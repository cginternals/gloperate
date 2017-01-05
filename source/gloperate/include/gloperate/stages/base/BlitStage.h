
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec4.hpp>

#include <globjects/Framebuffer.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage for blitting from one framebuffer to another
*/
class GLOPERATE_API BlitStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        BlitStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage for blitting from one framebuffer to another"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<globjects::Framebuffer *>  sourceFBO;           ///< FBO containing the source attachments
    Input<glm::vec4>                 sourceViewport;      ///< Viewport for reading from source FBO
    Input<globjects::Framebuffer *>  destinationFBO;      ///< FBO with destination attachments
    Input<glm::vec4>                 destinationViewport; ///< Viewport for writing into destination FBO

    // Outputs
    Output<globjects::Framebuffer *> blittedFBO;          ///< FBO containing blitted result
    Output<bool>                     blitted;             ///< True if processed successfully


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
    BlitStage(Environment * environment, const std::string & name = "BlitStage");


protected:
    // Virtual Stage interface
    virtual void onProcess(AbstractGLContext * context) override;


};


} // namespace gloperate
