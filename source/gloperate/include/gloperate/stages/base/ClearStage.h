
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
    class Framebuffer;
    class Texture;
}


namespace gloperate
{


class AbstractDrawable;


/**
*  @brief
*    Stage that clears the screen with a background color
*/
class GLOPERATE_API ClearStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ClearStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that clears the screen with a background color"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface                  renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<globjects::Texture *>      colorTexture;    ///< Pass in of texture input/output

    // Outputs
    Output<globjects::Framebuffer *> fboOut;          ///< Pass through framebuffer
    Output<globjects::Texture *>     colorTextureOut; ///< Pass through color texture


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
    ClearStage(Environment * environment, const std::string & name = "ClearStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ClearStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * content) override;
};


} // namespace gloperate
