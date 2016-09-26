
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <glm/vec4.hpp>


namespace gloperate
{


class GLOPERATE_API MultiFrameControlStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameControlStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "" // TODO: Description
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    );


public:
    Input<int>       frameNumber;       ///< Total frame count
    Input<glm::vec4> viewport;          ///< Viewport

    Output<int>      currentFrame;      ///< Number of aggregated frames
    Output<float>    aggregationFactor; ///< Weight for next aggregated frame (= 1 / frameNumber)


public:
    MultiFrameControlStage(Environment * environment, const std::string & name = "MultiFrameControlStage");
    virtual ~MultiFrameControlStage();


protected:
    virtual void onProcess(AbstractGLContext * context) override;


protected:
    int m_currentFrame;
};


} // namespace gloperate
