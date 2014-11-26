#pragma once

#include <pipeline-painters/pipeline_painters_api.h>


#include <gloperate/pipeline/PipelinePainter.h>

#include <pipeline-painters/postprocessing/PostprocessingPipeline.h>


class PIPELINE_PAINTERS_API Postprocessing : public gloperate::PipelinePainter
{
public:
    Postprocessing(gloperate::ResourceManager & resourceManager);
    virtual ~Postprocessing();

protected:
    PostprocessingPipeline m_pipeline;
};
