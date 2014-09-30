#pragma once

#include <string>

#include <gloperate/gloperate_api.h>

namespace gloperate {

class AbstractStage;

class GLOPERATE_API StageOwned
{
    friend class AbstractStage;
public:
    StageOwned(const std::string & name = "");
    virtual ~StageOwned();

    const std::string & name() const;
    void setName(const std::string & name);

    bool hasName() const;

    std::string asPrintable() const;

    bool hasOwner() const;
    const AbstractStage * owner() const;

    virtual std::string qualifiedName() const;
protected:
    AbstractStage * m_owner;
    std::string m_name;

    void setOwner(AbstractStage * owner);
};

} // namespace gloperate
