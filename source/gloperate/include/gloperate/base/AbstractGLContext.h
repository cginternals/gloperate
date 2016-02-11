
#pragma once


#include <glbinding/ContextHandle.h>

#include <gloperate/base/GLContextFormat.h>
#include <gloperate/base/AbstractContext.h>


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
class GLOPERATE_API AbstractGLContext : public AbstractContext
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractGLContext();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractGLContext();

    /**
    *  @brief
    *    Get context handle
    *
    *  @return
    *    OpenGL context handle
    */
    virtual glbinding::ContextHandle handle() const = 0;

    /**
    *  @brief
    *    Get context format
    *
    *  @return
    *    OpenGL context format
    */
    virtual const GLContextFormat & format() const = 0; 
};


} // namespace gloperate
