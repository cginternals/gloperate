#pragma once

#include <gloperate/pipeline/PipelinePainter.h>

#include "PostprocessingPipeline.h"


class Postprocessing : public gloperate::PipelinePainter
{
public:
    Postprocessing(gloperate::ResourceManager & resourceManager);

protected:
    PostprocessingPipeline m_pipeline;
};
