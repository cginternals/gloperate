#pragma once

#include <glm/vec2.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>


namespace gloperate
{


class GLOPERATE_API MultiFrameDiscDistributionStage : public gloperate::Stage
{
public:
    MultiFrameDiscDistributionStage(gloperate::Environment * environment, const std::string & name = "MultiFrameDiscDistributionStage");

public:
    Input<bool> isActive;
    Input<float> radius;

    Input<int> currentMultiFrame;
    Input<int> multiFrameCount;

    Output<glm::vec2> value;

protected:
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

};


} // namespace gloperate
