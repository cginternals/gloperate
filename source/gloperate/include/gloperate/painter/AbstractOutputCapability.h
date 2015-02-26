#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/pipeline/Data.h>

#include <gloperate/painter/AbstractCapability.h>

#include <string>
#include <vector>


namespace gloperate
{

class AbstractData;

/**
*  @brief
*    Base class for output capabilities
*
*/
class GLOPERATE_API AbstractOutputCapability : public gloperate::AbstractCapability
{

public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractOutputCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractOutputCapability();

    virtual std::vector<gloperate::AbstractData*> findOutputs(const std::string & name) const;


    template <typename T>
    Data<T> * getOutput(const std::string & name) const;
    template <typename T>
    Data<T> * getOutput() const;

    virtual std::vector<gloperate::AbstractData*> allOutputs() const = 0;
};

} // namespace gloperate

#include <gloperate/painter/AbstractOutputCapability.hpp>