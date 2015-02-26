#include <gloperate/painter/PipelineOutputCapability.h>

#include <gloperate/base/collection.hpp>

#include <gloperate/pipeline/AbstractData.h>
#include <gloperate/pipeline/AbstractPipeline.h>
#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/Data.h>

namespace gloperate
{

/**
*  @brief
*    Constructor
*/
PipelineOutputCapability::PipelineOutputCapability(gloperate::AbstractPipeline & pipeline)
    : AbstractOutputCapability()
    , m_pipeline(pipeline)
{
}

/**
*  @brief
*    Destructor
*/
PipelineOutputCapability::~PipelineOutputCapability()
{
}

std::vector<gloperate::AbstractData*> PipelineOutputCapability::allOutputs() const
{
    return m_pipeline.allOutputs();
}

}
