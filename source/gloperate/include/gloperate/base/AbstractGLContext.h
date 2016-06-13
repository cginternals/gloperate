
#pragma once


#include <glbinding/ContextHandle.h>

#include <gloperate/base/AbstractContext.h>
#include <gloperate/base/GLContextFormat.h>


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
    *
    *  @remarks
    *    Note for derived classes:
    *    The data member m_handle has to be initialized to
    *    the correct value when creating a context.
    *    It is not retrieved automatically by this function.
    */
    glbinding::ContextHandle handle() const;

    /**
    *  @brief
    *    Get context format
    *
    *  @return
    *    OpenGL context format
    *
    *  @remarks
    *    Note for derived classes:
    *    The data member m_format has to be initialized to
    *    the correct value when creating a context.
    *    It is not retrieved automatically by this function.
    */
    const GLContextFormat & format() const;


protected:
    glbinding::ContextHandle m_handle;  ///< OpenGL context handle
    GLContextFormat          m_format;  ///< OpenGL context format
};


} // namespace gloperate
