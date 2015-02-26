#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractOutputCapability.h>
#include <gloperate/pipeline/AbstractPipeline.h>

#include <string>
#include <vector>


namespace gloperate
{

class AbstractData;
template <typename T>
class Data;

/**
*  @brief
*    OutputCapability for pipelines
*
*/
class GLOPERATE_API PipelineOutputCapability : public AbstractOutputCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    PipelineOutputCapability(gloperate::AbstractPipeline & pipeline);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PipelineOutputCapability();

    virtual std::vector<gloperate::AbstractData*> allOutputs() const;

protected:
    gloperate::AbstractPipeline & m_pipeline;
};

} // namespace gloperate
