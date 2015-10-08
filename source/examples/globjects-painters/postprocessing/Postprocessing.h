
#pragma once


#include <gloperate/pipeline/PipelinePainter.h>

#include "PostprocessingPipeline.h"


class Postprocessing : public gloperate::PipelinePainter
{
public:
    Postprocessing(gloperate::ResourceManager & resourceManager, const reflectionzeug::Variant & pluginInfo);

    bool animation() const;
    void setAnimation(const bool & enabled);


protected:
    PostprocessingPipeline m_pipeline;

    bool m_animation;
};
