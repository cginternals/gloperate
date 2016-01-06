
#include <gloperate/painter/PipelineOutputCapability.h>

#include <gloperate/base/collection.hpp>

#include <gloperate/pipeline/AbstractPipeline.h>


namespace gloperate
{


PipelineOutputCapability::PipelineOutputCapability(const gloperate::AbstractPipeline & pipeline)
    : AbstractOutputCapability{}
    , m_pipeline ( pipeline )
{
}

PipelineOutputCapability::~PipelineOutputCapability()
{
}

std::vector<gloperate::AbstractData*> PipelineOutputCapability::allOutputs() const
{
    return m_pipeline.allOutputs();
}


}
