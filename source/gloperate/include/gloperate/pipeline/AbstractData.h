
#pragma once


#include <string>

#include <signalzeug/Signal.h>

#include <gloperate/gloperate_api.h>


namespace gloperate 
{


class AbstractStage;


/**
*  @brief
*    Base class for typed data containers
*
*  @see
*    Data
*/
class GLOPERATE_API AbstractData
{
    friend class AbstractStage;


public:
    signalzeug::Signal<> invalidated;


public:
    AbstractData(const std::string & name = "");
    virtual ~AbstractData();

    const std::string & name() const;
    void setName(const std::string & name);

    bool hasName() const;

    std::string asPrintable() const;

    bool hasOwner() const;
    const AbstractStage * owner() const;

    virtual std::string qualifiedName() const;

    void invalidate();

    bool matchesName(const std::string & name) const;

    virtual std::string type() const = 0;


protected:
    void setOwner(AbstractStage * owner);


protected:
    AbstractStage * m_owner;
    std::string m_name;
};


} // namespace gloperate
