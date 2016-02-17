
#pragma once


#include <gloperate/viewer/TimeManager.h>


namespace gloperate
{


/**
*  @brief
*    Context containing the top-level components needed for rendering
*
*    The viewer context holds all necessary components used for the rendering process,
*    such as the plugin manager, input manager, resource loaders, etc. It should
*    usually be instanciated once by the top-level viewer application and passed
*    down to other objects that need access these components. Some components
*    have to be provided by the windowing backend on creation of the viewer context.
*
*    A viewer context is independend from windows and OpenGL contexts and should be
*    initialized before creating actual viewers and their OpenGL contexts.
*/
class GLOPERATE_API ViewerContext
{
public:
    //@{
    /**
    *  @brief
    *    Constructor
    */
    ViewerContext();

    /**
    *  @brief
    *    Destructor
    */
    ~ViewerContext();
    //@}

    //@{
    /**
    *  @brief
    *    Get time manager
    *
    *  @return
    *    Time manager (must NOT be null)
    */
    const TimeManager * timeManager() const;
    TimeManager * timeManager();
    //@}


protected:
    TimeManager m_timeManager; ///< Manager for virtual time and timers
};


} // namespace gloperate
