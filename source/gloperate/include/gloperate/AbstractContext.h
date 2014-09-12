#pragma once

#include <string>
#include <vector>

#include <glbinding/ContextHandle.h>

#include <gloperate/gloperate_api.h>
#include <gloperate/ContextFormat.h>


namespace gloperate
{

class GLOPERATE_API AbstractContext
{
public:
    enum class SwapInterval 
    {
        NoVerticalSyncronization       =  0
    ,   VerticalSyncronization         =  1
    ,   AdaptiveVerticalSyncronization = -1
    };

    static const std::string & swapIntervalString(SwapInterval swapInterval);

public:
    AbstractContext();
    virtual ~AbstractContext();

    // returns the context handle - if this is somehow encapsulated tryFetchHandle for retrieval.
    virtual glbinding::ContextHandle handle() const = 0;

	// this should be in sync to the created context, not the requested one
    virtual const ContextFormat & format() const = 0; 

	// returns true if the context was created (irrespective of format verification)
	// and if handle() returns handle > 0.
    virtual bool isValid() const;

	// swap interval relates to context, since there might be multiple 
	// shared contexts with same format, but individual swap format.
    SwapInterval swapInterval() const;
	virtual bool setSwapInterval(SwapInterval interval);

protected:
	// operates by making the context current and fetching its context handle
    // NOTE: this is not thread safe, so this might result in errorneuos behavior or wrong handle.
	glbinding::ContextHandle tryFetchHandle();

protected:
	SwapInterval m_swapInterval;
};

} // namespace gloperate
