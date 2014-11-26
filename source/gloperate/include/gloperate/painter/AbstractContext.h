#pragma once

#include <string>
#include <vector>

#include <glbinding/ContextHandle.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/ContextFormat.h>


namespace gloperate
{

class GLOPERATE_API AbstractContext
{
public:
    /** note: requires that glfw::init was previously called 
        note: requires active context
    */
    static glbinding::Version retrieveVersion();

    enum class SwapInterval 
    {
        NoVerticalSyncronization       =  0
    ,   VerticalSyncronization         =  1
    ,   AdaptiveVerticalSyncronization = -1
    };

    static const std::string & swapIntervalString(SwapInterval swapInterval);


    /** strings for version, vendor, and renderer information.
        note: requires active context
    */
    static std::string version();
    static std::string vendor();
    static std::string renderer();

public:
    AbstractContext();
    virtual ~AbstractContext();

    /** returns the context's handle - if this is somehow encapsulated tryFetchHandle for retrieval.
    */
    virtual glbinding::ContextHandle handle() const = 0;

	/** this should be in sync to the created context, not the requested one
    */
    virtual const ContextFormat & format() const = 0; 

	/** returns true if the context was created (irrespective of format verification)
	    and if handle() returns handle > 0.
    */
    virtual bool isValid() const;

	/** swap interval relates to context, since there might be multiple 
	    shared contexts with same format, but individual swap format.
    */
    SwapInterval swapInterval() const;
    /** note: requires active context
    */
	virtual bool setSwapInterval(SwapInterval interval);

    virtual void makeCurrent() const = 0;
    virtual void doneCurrent() const = 0;

protected:
	/** operates by making the context current and fetching its context handle
        note: this is not thread safe, so this might result in errorneuos behavior or wrong handle.
    */
	static glbinding::ContextHandle tryFetchHandle();

protected:
	SwapInterval m_swapInterval;
};

} // namespace gloperate
