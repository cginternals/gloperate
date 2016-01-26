
#pragma once


#include <gloperate/input/input.h>
#include <gloperate/painter/AbstractCapability.h>


namespace gloperate 
{


/**
*  @brief
*    Capability that enables receiving direct mouse and keyboard events
*
*    If a painter supports this capability, it will be supplied with mouse
*    and keyboard events from the viewer. Usually, the abstract interaction
*    interfaces should be used instead, but this capability is important for
*    implementing interoperability with other renderig systems.
*/
class GLOPERATE_API AbstractInputCapability : public AbstractCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractInputCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractInputCapability();

    /**
    *  @brief
    *    Called on mouse move
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    */
    virtual void onMouseMove(int x, int y) = 0;

    /**
    *  @brief
    *    Called on mouse pressed
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    *  @param[in] button
    *    Mouse button
    */
    virtual void onMousePress(int x, int y, gloperate::MouseButton button) = 0;

    /**
    *  @brief
    *    Called on mouse released
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    *  @param[in] button
    *    Mouse button
    */
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button) = 0;

    /**
    *  @brief
    *    Called on mouse double-click
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    *  @param[in] button
    *    Mouse button
    */
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button) = 0;

    /**
    *  @brief
    *    Called on mouse wheel event
    *
    *  @param[in] dx
    *    X delta
    *  @param[in] y
    *    Y delta
    */
    virtual void onMouseWheel(int dx, int dy) = 0;

    /**
    *  @brief
    *    Called on key down
    *
    *  @param[in] key
    *    Key
    */
    virtual void onKeyDown(gloperate::Key key) = 0;

    /**
    *  @brief
    *    Called on key up
    *
    *  @param[in] key
    *    Key
    */
    virtual void onKeyUp(gloperate::Key key) = 0;
};


} // namespace gloperate
