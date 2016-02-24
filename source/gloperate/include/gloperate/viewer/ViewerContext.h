
#pragma once


#include <vector>

#include <gloperate/viewer/TimeManager.h>


namespace gloperate
{


class Surface;


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
    friend class Surface;


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

    /**
    *  @brief
    *    Update timing
    *
    *  @return
    *    'true' if there are any timers active, 'false' if not
    *
    *  @remarks
    *    This signature measures the time since the last call
    *    for the time delta. It should usually be used in
    *    interactive applications.
    */
    bool update();

    /**
    *  @brief
    *    Update timing
    *
    *  @param[in] delta
    *    Time delta (in seconds)
    *
    *  @return
    *    'true' if there are any timers active, 'false' if not
    *
    *  @remarks
    *    This signature can be used to provide a specific time delta,
    *    e.g., when rendering videos at a fixed frame rate.
    */
    bool update(float delta);


protected:
    //@{
    /**
    *  @brief
    *    Register render surface
    *
    *  @param[in] surface
    *    Render surface (must NOT be null!)
    */
    void registerSurface(Surface * surface);

    /**
    *  @brief
    *    Un-register render surface
    *
    *  @param[in] surface
    *    Render surface (must NOT be null!)
    */
    void unregisterSurface(Surface * surface);
    //@}


protected:
    TimeManager            m_timeManager; ///< Manager for virtual time and timers
    std::vector<Surface *> m_surfaces;    ///< List of active surfaces
};


} // namespace gloperate
