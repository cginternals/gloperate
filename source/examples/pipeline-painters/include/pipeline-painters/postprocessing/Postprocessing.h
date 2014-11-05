#pragma once

#include <gloperate/pipelines/PipelinePainter.h>

#include <pipeline-painters/postprocessing/PostprocessingPipeline.h>
#include <pipeline-painters/pipeline_painters_api.h>


class PIPELINE_PAINTERS_API Postprocessing : public gloperate::PipelinePainter
{
public:
    Postprocessing(gloperate::ResourceManager & resourceManager);
    virtual ~Postprocessing();

protected:
    PostprocessingPipeline m_pipeline;
};
