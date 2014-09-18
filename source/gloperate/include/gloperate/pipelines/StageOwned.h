#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/Nameable.h>

namespace gloperate {

class AbstractStage;

class GLOPERATE_API StageOwned : public Nameable
{
    friend class AbstractStage;
public:
    StageOwned();
    virtual ~StageOwned();

    bool hasOwner() const;
    const AbstractStage * owner() const;

    virtual std::string qualifiedName() const;
protected:
    AbstractStage * m_owner;

    void setOwner(AbstractStage * owner);
};

} // namespace gloperate
