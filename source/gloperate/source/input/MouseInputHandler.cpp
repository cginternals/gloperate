#include <gloperate/input/MouseInputHandler.h>


namespace gloperate
{
   
/**
*  @brief
*    Constructor
*/
MouseInputHandler::MouseInputHandler()
{
}

/**
*  @brief
*    Destructor
*/
MouseInputHandler::~MouseInputHandler()
{
}

/**
*  @brief
*    Called on mouse move
*/
void MouseInputHandler::onMouseMove(int /*x*/, int /*y*/)
{
}

/**
*  @brief
*    Called on mouse pressed
*/
void MouseInputHandler::onMousePress(int /*x*/, int /*y*/, gloperate::MouseButton /*button*/)
{
}

/**
*  @brief
*    Called on mouse released
*/
void MouseInputHandler::onMouseRelease(int /*x*/, int /*y*/, gloperate::MouseButton /*button*/)
{
}

/**
*  @brief
*    Called on mouse double-click
*/
void MouseInputHandler::onMouseDoubleClick(int /*x*/, int /*y*/, gloperate::MouseButton /*button*/)
{
}

/**
*  @brief
*    Called on mouse wheel event
*/
void MouseInputHandler::onMouseWheel(int /*dx*/, int /*dy*/)
{
}

} // gloperate
