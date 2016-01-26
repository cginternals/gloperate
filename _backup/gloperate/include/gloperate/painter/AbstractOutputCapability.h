
#pragma once


#include <string>
#include <vector>

#include <gloperate/painter/AbstractCapability.h>


namespace gloperate
{


class AbstractData;
template<typename T>

class Data;


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
