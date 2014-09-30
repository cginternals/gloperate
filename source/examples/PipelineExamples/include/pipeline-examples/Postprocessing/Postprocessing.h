#pragma once

#include <gloperate/pipelines/PipelinePainter.h>

#include <pipeline-examples/Postprocessing/PostprocessingPipeline.h>

#include <pipeline-examples/pipeline_examples_api.h>

class PIPELINE_EXAMPLES_API Postprocessing : public gloperate::PipelinePainter
{
public:
    Postprocessing(gloperate::ResourceManager & resourceManager);
    virtual ~Postprocessing();
protected:
    PostprocessingPipeline m_pipeline;
};
