
#pragma once


#include <string>
#include <vector>

#include <glbinding/ContextHandle.h>

#include <gloperate/gloperate_api.h>
#include <gloperate/viewer/ContextFormat.h>


namespace gloperate
{


/**
*  @brief
*    Abstract base class for accessing an OpenGL context
*
*    This is an abstract interface that allows for accessing and controlling an OpenGL context.
*    It is to be subclassed by specific backends, which serve as an interface to their respective
*    windowing framework that was used to create the OpenGL context and/or window.
*    Have a look at gloperate-qt or gloperate-glfw to find default implementations for the respective framework.
*/
class GLOPERATE_API AbstractContext
{
public:
    /**
    *  @brief
    *    Vertical synchronization options
    *
    *    [TODO] SwapInterval seems to be the wrong name, as it does not specify any interval. VerticalSync seems appropriate.
    */
    enum class SwapInterval 
    {
        // [TODO] Synchronization misses an 'h'
        NoVerticalSyncronization       =  0 /**< Disable vertical synchronization */
    ,   VerticalSyncronization         =  1 /**< Enable vertical synchronization */
    ,   AdaptiveVerticalSyncronization = -1 /**< Use adaptive vertical synchronization */
    };


public:
    /**
    *  @brief
    *    Get OpenGL version
    *
    *  @notes
    *    - Requires active context
    *    - Requires that glfw::init was previously called
    *      [TODO] This is an abstract base class, it shouldn't be dependent on GLFW!
    */
    static glbinding::Version retrieveVersion();

    /**
    *  @brief
    *    Get swap interval option as string
    *
    *  @param[in] swapInterval
    *    Swap interval option
    *
    *  @return
    *    Name of swap interval option
    */
    static const std::string & swapIntervalString(SwapInterval swapInterval);

    /**
    *  @brief
    *    Get OpenGL version
    *
    *  @return
    *    OpenGL version string
    *
    *  @notes
    *    - Requires active context
    */
    static std::string version();

    /**
    *  @brief
    *    Get OpenGL vendor
    *
    *  @return
    *    OpenGL vendor string
    *
    *  @notes
    *    - Requires active context
    */
    static std::string vendor();

    /**
    *  @brief
    *    Get OpenGL renderer
    *
    *  @return
    *    OpenGL renderer string
    *
    *  @notes
    *    - Requires active context
    */
    static std::string renderer();


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractContext();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractContext();

    /**
    *  @brief
    *    Check if context is valid
    *
    *  @return
    *    'true' if the context was created (irrespective of format verification) and handle() > 0, else 'false'
    */
    virtual bool isValid() const;

    /**
    *  @brief
    *    Get context handle
    *
    *  @return
    *    OpenGL context handle
    *
    *  @remarks
    *    [TODO] I do not understand this comment, please clarify
    *    If this is somehow encapsulated tryFetchHandle for retrieval.
    */
    virtual glbinding::ContextHandle handle() const = 0;

    /**
    *  @brief
    *    Get context format
    *
    *  @return
    *    OpenGL context format
    *
    *  @remarks
    *    Note for derived classes: the returned format should be in sync to the created context, not the requested one
    */
    virtual const ContextFormat & format() const = 0; 

    /**
    *  @brief
    *    Get swap interval option
    *
    *  @return
    *    Swap interval option
    *
    *  @remarks
    *    The swap interval relates to a context, since there might be multiple 
    *    shared contexts with same format, but individual swap formats.
    */
    SwapInterval swapInterval() const;

    /**
    *  @brief
    *    Set swap interval option
    *
    *  @param[in] interval
    *    Swap interval option
    *
    *  @notes
    *    - Requires active context.
    */
    virtual bool setSwapInterval(SwapInterval interval);

    /**
    *  @brief
    *    Make context current
    */
    virtual void makeCurrent() const = 0;

    /**
    *  @brief
    *    Release context
    */
    virtual void doneCurrent() const = 0;


protected:
    /**
    *  @brief
    *    Try to fetch the handle of this context (not thread safe!)
    *
    *  @return
    *    Context handle
    *
    *  @remarks
    *    Operates by making the context current and fetching its context handle.
    *    This is not thread safe, so it might result in erroneuos behavior or wrong handle.
    */
    static glbinding::ContextHandle tryFetchHandle();


protected:
    SwapInterval m_swapInterval; ///< Swap interval option
};


} // namespace gloperate
