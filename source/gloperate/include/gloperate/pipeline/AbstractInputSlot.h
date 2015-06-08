#pragma once

#include <string>

#include <gloperate/gloperate_api.h>

#include <signalzeug/Signal.h>


namespace gloperate 
{

class AbstractData;
class AbstractStage;


class GLOPERATE_API AbstractInputSlot
{
    friend class AbstractStage;

public:
    AbstractInputSlot(const std::string & name = "");
    virtual ~AbstractInputSlot();

    const std::string & name() const;
    void setName(const std::string & name);

    bool hasName() const;

    std::string asPrintable() const;

    bool hasOwner() const;
    const AbstractStage * owner() const;

    virtual std::string qualifiedName() const;

    virtual bool connectTo(const AbstractData & data) = 0;
    virtual bool matchType(const AbstractData & data) = 0;

    bool hasChanged() const;

    void changed();
    void processed();

    bool isUsable() const;

    bool isOptional() const;
    void setOptional(bool optional);

    bool isFeedback() const;
    void setFeedback(bool isFeedback);

    virtual const AbstractData * connectedData() const = 0;

    bool isConnected() const;

public:
    signalzeug::Signal<> connectionChanged;

protected:
    AbstractStage * m_owner;
    std::string m_name;

    bool m_hasChanged;
    bool m_isOptional;
    bool m_isFeedback;

    void setOwner(AbstractStage * owner);
};

} // namespace gloperate
