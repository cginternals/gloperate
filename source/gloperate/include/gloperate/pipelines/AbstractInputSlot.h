#pragma once

#include <gloperate/signals/Signal.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/StageOwned.h>

namespace gloperate {

class AbstractData;
	
class GLOPERATE_API AbstractInputSlot : public StageOwned
{
public:
	AbstractInputSlot();
	virtual ~AbstractInputSlot();

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
    gloperate::Signal<> connectionChanged;
protected:
	bool m_hasChanged;
	bool m_isOptional;
	bool m_isFeedback;
};

} // namespace gloperate
