
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


class AbstractGLContextFactory;


/**
*  @brief
*    Context containing the top-level components needed for rendering
*
*    The viewer context holds all necessary components used for the rendering process,
*    such as the plugin manager, input manager, resource loaders, etc. It should
*    usually be instanciated once by the top-level viewer application and passed
*    down to other objects that need access these components. Some components,
*    such as the context factory, have to be provided by the windowing backend.
*
*    A viewer context is independend from windows and OpenGL contexts and should be
*    initialized before creating actual viewers and their OpenGL contexts.
*/
class GLOPERATE_API ViewerContext
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] contextFactory
    *    OpenGL context factory
    */
    ViewerContext(AbstractGLContextFactory & contextFactory);

    /**
    *  @brief
    *    Destructor
    */
    ~ViewerContext();

    /**
    *  @brief
    *    Get context factory
    *
    *  @return
    *    Context factory
    */
    AbstractGLContextFactory & contextFactory();


protected:
    AbstractGLContextFactory & m_contextFactory;
};


} // namespace gloperate
