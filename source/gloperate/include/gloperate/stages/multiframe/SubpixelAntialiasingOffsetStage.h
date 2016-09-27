#pragma once

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace gloperate
{


class GLOPERATE_API SubpixelAntialiasingOffsetStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        SubpixelAntialiasingOffsetStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides random offsets for subpixel antialiasing"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )

public:
    SubpixelAntialiasingOffsetStage(gloperate::Environment * environment, const std::string & name = "SubpixelAntialiasingOffsetStage");

public:
    Input<glm::vec4> viewport;
    Input<int> currentMultiFrame;
    Input<int> multiFrameCount;

    Output<glm::vec2> subPixelOffset;

protected:
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

};


} // namespace gloperate
