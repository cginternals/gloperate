
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Abstract base class for contexts
*
*    This base class describes a common interface for contexts of any kind.
*    It is not meant for just OpenGL contexts, but can also be used for other
*    kind of contexts, such as CUDA, OpenAL, etc.
*/
class GLOPERATE_API AbstractContext
{
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
    *    Activate context
    */
    virtual void use() const = 0;

    /**
    *  @brief
    *    Release context
    */
    virtual void release() const = 0;
};


} // namespace gloperate
