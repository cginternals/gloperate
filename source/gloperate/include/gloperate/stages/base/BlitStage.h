
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
    Input<globjects::Framebuffer *>  sourceFBO;
    Input<glm::vec4>                 sourceViewport;
    Input<globjects::Framebuffer *>  destinationFBO;
    Input<glm::vec4>                 destinationViewport;

    Output<globjects::Framebuffer *> blittedFBO;
    Output<bool>                     blitted;


public:
    BlitStage(Environment * environment, const std::string & name = "BlitStage");

    virtual ~BlitStage();

protected:
    virtual void onProcess(AbstractGLContext * context) override;


};


}
