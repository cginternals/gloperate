#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractCapability.h>
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
*    Base class for output capabilities
*
*/
class GLOPERATE_API OutputCapability : public gloperate::AbstractCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    OutputCapability(gloperate::AbstractPipeline & pipeline);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OutputCapability();

    std::vector<gloperate::AbstractData*> findOutputs(const std::string & name) const;

    template <typename T>
    Data<T> * getOutput(const std::string & name) const;
    template <typename T>
    Data<T> * getOutput() const;


    std::vector<gloperate::AbstractData*> allOutputs() const;

protected:
    gloperate::AbstractPipeline & m_pipeline;
};

} // namespace gloperate

#include <gloperate/painter/OutputCapability.hpp>