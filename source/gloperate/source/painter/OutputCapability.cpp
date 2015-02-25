#include <gloperate/painter/OutputCapability.h>

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
OutputCapability::OutputCapability(gloperate::AbstractPipeline & pipeline)
    : AbstractCapability()
    , m_pipeline(pipeline)
{
}

/**
*  @brief
*    Destructor
*/
OutputCapability::~OutputCapability()
{
}

std::vector<gloperate::AbstractData*> OutputCapability::findOutputs(const std::string & name) const
{
    return m_pipeline.findOutputs(name);
}

}